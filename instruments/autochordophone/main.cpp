extern "C" {
#include "mGeneral.h"
#include "mBus.h"
#include "mUSB.h"
#include "stdio.h"
#include "stm32f37x.h"
#include "mEasy.h"
//#include "stepper.h"
#include <math.h>
#include "arm_math.h"
#include "Stepper.h"
}

#define rest 1
#define C -50
#define Df -100
#define D -150
#define Ef -200
#define E -250
#define F -300
#define Gf -350
#define G -400
#define Af -550
#define A -600
#define Bf -650
#define B -700

volatile uint16_t p=0;
volatile uint16_t T5flag=1;

uint8_t a;
int8_t dir;
int8_t dir_cmd=1;
volatile int pose=1;
int k=0;

int test2=0;
int f=0;



void picker_setup(void);
void strum(void);
void damp(void);



void hold(uint16_t T){ //Timer up to 1 seconds. (currently)
    
    TIM5->ARR=T*15000;
    TIM5->CNT=0;
    TIM_Cmd(TIM5, ENABLE);
}

void strum_speed(float speed){
    TIM5->ARR=60000*speed;
    //        TIM5->CNT=0;
}


int main( void )
{
    mInit();
    mUSBInit();
    mBusInit();
    
    picker_setup();
    stepper_setup();
    set_timer5 (60000,30000,1200);
    
    setMode( STEP_FULL);
    setSpeedLimits( 65, 650 );
    set_speed(m_speed_min);
    setAccelerationLimit(1000); //5000 is good
    
    
    int test=0;
    
    uint16_t pos[]={0,-180,0,-360,0,-540,0,-720,-900,0};
    uint16_t dur[]={1000,1000,1000,1000};
    
    set_speed(m_speed_min);
    float i=0;
    while(1)
    {
        
        
        
        test = update(pos[test2]);
        if(test==1){
            mRedOFF;
            strum_speed(1-i/11);
            test2++;
            i++;
            
            //            mWaitms(100);
        }
        if(test2>=11){
            test2=11;
            mWhiteON;
        }
    }
    return(0);
}


void picker_setup(void){
    /*Picket between 2000, and 6600
     Arm between 1500 and 4000
     Arm Picks between 2800 and 3200
     */
    set_gpio(GPIOA,GPIO_Pin_4,MODE_ALT_FUNCTION); //Step Pulse (Mode 3 for pwm)
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_10);//Connect Pin to Timer
    set_gpio(GPIOA,GPIO_Pin_5,MODE_ALT_FUNCTION); //Step Pulse (Mode 3 for pwm)
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_10);//Connect Pin to Timer
    set_pwm_servo(60000,30000,30);
}

void strum(void){
    
    switch(pose){
        case 1:
            modulatePick(3400);
            modulateArm(2075);
            pose=2;
            break;
        case 2:
            modulatePick(4000);
            modulateArm(2450);
            pose=1;
            break;
        default:
            mWhiteON;
            mWaitms(100);
            mWhiteOFF;
            break;
    }
}
void damp(void){
    modulatePick(4900);
    modulateArm(2870);
}
/*PWM ON TIM 12 PIN A4 CH 2*/


void TIM5_IRQHandler(void)
{
    mRedON;
    strum();
    TIM_ClearFlag(TIM5,TIM_FLAG_CC1);
    TIM5->CNT=0;
}
void TIM12_IRQHandler(void)
{
    TIM_ClearFlag(TIM12,TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4);
}
