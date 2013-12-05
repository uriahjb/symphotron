/*
 Stepper Implmentation
 */

#include "mEasy.h"
#include "Stepper.h"
#include <stdlib.h>
#include <math.h>
#include "arm_math.h"

void stepper_setup(void){
    set_gpio(GPIOA,GPIO_Pin_8,MODE_OUT); //Mode 2 Pin
    set_gpio(GPIOA,GPIO_Pin_9,MODE_OUT); //Mode 1 Pin
    set_gpio(GPIOA,GPIO_Pin_10,MODE_OUT); //Mode 0 Pin
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
    GPIO_ResetBits(GPIOA,GPIO_Pin_9);
    GPIO_ResetBits(GPIOA,GPIO_Pin_10);
    
    set_gpio(GPIOB,GPIO_Pin_4,MODE_OUT); //Mode 1 Pin
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    
    //    set_gpio(GPIOB,GPIO_Pin_3,MODE_OUT); //Mode 1 Pin
    //    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
    set_gpio(GPIOB,GPIO_Pin_3,MODE_ALT_FUNCTION); //Mode 0 Pin
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1);//Connect Pin to Timer
    set_pwm_stepper(60000,4000,Clock_Divide);
    set_acc_timer(60000,1000,12); //INT every 0.01 sec @ 60000 (change CNT1));
}
void setMode( int mode ) {
    // Set m_mode to actual divisor
    m_mode = ( 1 << mode );
    
    switch(mode){
        case 0:
            GPIO_ResetBits(GPIOA,GPIO_Pin_8);
            GPIO_ResetBits(GPIOA,GPIO_Pin_9);
            GPIO_ResetBits(GPIOA,GPIO_Pin_10);
            break;
        case 1:
            GPIO_ResetBits(GPIOA,GPIO_Pin_8);
            GPIO_ResetBits(GPIOA,GPIO_Pin_9);
            GPIO_SetBits(GPIOA,GPIO_Pin_10);
            break;
        case 2:
            GPIO_ResetBits(GPIOA,GPIO_Pin_8);
            GPIO_SetBits(GPIOA,GPIO_Pin_9);
            GPIO_ResetBits(GPIOA,GPIO_Pin_10);
            break;
        case 3  :
            GPIO_ResetBits(GPIOA,GPIO_Pin_8);
            GPIO_SetBits(GPIOA,GPIO_Pin_9);
            GPIO_SetBits(GPIOA,GPIO_Pin_10);
            break;
        case 4:
            GPIO_SetBits(GPIOA,GPIO_Pin_8);
            GPIO_ResetBits(GPIOA,GPIO_Pin_9);
            GPIO_ResetBits(GPIOA,GPIO_Pin_10);
            break;
        case 5:
            GPIO_SetBits(GPIOA,GPIO_Pin_8);
            GPIO_ResetBits(GPIOA,GPIO_Pin_9);
            GPIO_SetBits(GPIOA,GPIO_Pin_10);
            break;
        default:
            GPIO_ResetBits(GPIOA,GPIO_Pin_8);
            GPIO_ResetBits(GPIOA,GPIO_Pin_9);
            GPIO_ResetBits(GPIOA,GPIO_Pin_10);
            break;
    }
}
void start( void ) {
    TIM_Cmd(TIM2, ENABLE);
}
void stop( void ) {
    TIM_Cmd(TIM2, DISABLE);
}
void accStart(void){
    TIM_Cmd(TIM4, ENABLE);
    Aflag=1;
}
void accStop(void){
    TIM_Cmd(TIM4, DISABLE);
    Aflag=0;
}

void setPosition( int32_t desired_position ) {
    m_desired_position = (float)400*desired_position/360; //multiply by mode
}
void set_speed(uint16_t speed){
    m_speed = speed;
    uint16_t A = ((1.8/(float)m_mode)/(float)speed)*((72000000)/(float)Clock_Divide); //30 is the PWM clock divide
    TIM2->ARR=A;
    if(TIM2->ARR<TIM2->CNT)
        TIM2->CNT=0;
    
}
void set_direction(int dir){
    m_dir=dir;
    if(dir>0)
        GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    else
        GPIO_SetBits(GPIOB,GPIO_Pin_4);
}
void setAccelerationLimit( float acceleration ) {
    m_acceleration_limit = ((1/(float)acceleration) * (float)(6*pow(10,6)));
    TIM4->ARR=m_acceleration_limit;
    if(TIM4->ARR<TIM4->CNT)
        TIM4->CNT=0;
}


uint32_t getCount( void ) {
    return m_cnt;
}
void updateCount(void){
    if(m_dir>0){
        m_cnt++;
    }
    else{
        m_cnt--;
    }
    
}
int getModeDivisor( void ) {
    return m_mode;
}
float getAccelerationLimit( void ) {
    return ((1/m_acceleration_limit) * (6*pow(10,6)));
}
float getSpeed( void ) {
    return m_speed;
}
int getPosition( void ) {
    return m_cnt;
}

void setSpeedLimits( float speed_min, float speed_max ) {
    m_speed_min = speed_min;
    m_speed_max = speed_max;
}



int update( int16_t Note ) {
    setPosition(Note*m_mode);
    error = (float)(m_desired_position - m_cnt);
    error_dir = error > 0.0f ? 1.0f : -1.0f;
    
    if(abs(error) <= 0){
        com1=1;
    }else{
        com1=2;
    }
    switch(com1){
        case 1:
            stop();
            return(1);
            break;
        case 2:
            start();
            set_direction(error_dir);
            acceleration=getAccelerationLimit();
            t = m_speed/acceleration;
            // Compute distance required to deccelerate
            dec_dist = 0.5f*acceleration*(t*t)*m_mode; //Number at end = Guess
            // If error is <= dec_dist then decellerate, otherwise accelerate
            
            if ( abs(error) <= dec_dist ) {
                mBlueON;
                mYellowOFF;
                if(m_speed<=m_speed_min){ //64 = minspeed
                    if(Aflag!=0)
                        accStop();
                    //                    set_speed(m_speed_min);
                }else{
                    accel=0;
                    if(Aflag!=1)
                        accStart();
                }
            }else{
                mBlueOFF;
                mYellowON;
                if(m_speed<=m_speed_max){
                    accel=1;
                    if(Aflag!=1)
                        accStart();
                }else{
                    if(Aflag!=0)
                        accStop();
                }
            }
            break;
        default:
            break;
    }
    return(0);
}

void TIM2_IRQHandler(void)
{
    updateCount();
    TIM_ClearFlag(TIM2,TIM_FLAG_CC2);
}
void TIM4_IRQHandler(void)
{
    if(accel == 1){
        set_speed(m_speed+1);
    }else{
        set_speed(m_speed-1);
    }
    TIM_ClearFlag(TIM4,TIM_FLAG_CC1);
}