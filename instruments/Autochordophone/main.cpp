// These are C things extern C so we can mix C and CPP
extern "C" {
#include "mGeneral.h"
#include "mBus.h"
#include "mUSB.h"
#include "stdio.h"
#include "stm32f37x.h"
#include "mEasy.h"
}
//#include "Stepper.h"

//#include "bqueue.h"
//#include "usb_iface.h"
uint8_t a;
int8_t dir;
int8_t dir_cmd=1;
int pose=1;
int k=0;
uint32_t p=4000;


void picker_setup(void);
void stepper_setup(void);
void strum(void);
void damp(void);

int main( void )
{
    mInit();
    mUSBInit();
    mBusInit();
    
    
    picker_setup();
    //    stepper_setup();
    
    
    
    
    //    uint16_t ii = 0x0001;
    uint16_t i;
    uint16_t j;
    while(1)
    {
        //        for(j = 0 ; j<100; j++){
        //            for(i = 0 ; i<18000;i++){
        //                mGreenON;
        //            }
        //        }
        //        mRedON;
        //        //        TIM2->PSC = ii;
        //        //        ii++;
        //        for(j = 0 ; j<100; j++){
        //            for(i = 0 ; i<18000;i++){
        //                mGreenOFF;
        //            }
        //        }
        //        mRedOFF;
        
        //        TIM2->PSC = ii;
        //        ii++;
        
        
//        for(k=0;k<=8;k++){
//            mRedON;
//            strum();
//            mWaitms(50);
//            mYellowON;
//        }
//        mRedOFF;
//        damp();
//        mWaitms(1000);
//        GPIO_ResetBits(GPIOB,GPIO_Pin_3);
//        mWhiteON;
//        mWaitms(5);
//        GPIO_SetBits(GPIOB,GPIO_Pin_3);
//        mWhiteOFF;
//        mWaitms(5);
        
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
    set_pwm_servo(60000,2000,24);
}
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
    
    
    
    set_pwm_servo(60000,2000,24);
    //    set_pwm_stepper(60000,30000,24);
    
    /*Have to make a more general PWM Function Creator*/
    
}
void strum(void){
    
    switch(pose){
        case 1:
            modulatePick(4400);
            modulateArm(2700);
            pose=2;
            break;
        case 2:
            modulatePick(5400);
            modulateArm(3100);
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

void TIM2_IRQHandler(void)
{
    mYellowON;
    TIM_ClearFlag(TIM2,TIM_FLAG_CC2);
}
void TIM12_IRQHandler(void)
{
    mYellowON;
    TIM_ClearFlag(TIM12,TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4);
}
