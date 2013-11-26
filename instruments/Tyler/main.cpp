`// These are C things extern C so we can mix C and CPP
extern "C" {
#include "mBus.h"
#include "mUSB.h"
#include "mZigbee.h"
#include "stdio.h"
#include "stm32f37x.h"
//#include "mEasy.h"
}
uint8_t a;
int8_t dir;
int8_t dir_cmd=1;
int pose=1;

//void picker_setup(void);
//void stepper_setup(void);
//void strum(void);
//void damp(void);


uint32_t p=4000;
int main( void )
{
    mInit();
    mUSBInit();
    mBusInit();
//    picker_setup();
    mWaitms(250);
    mWhiteON;
    
    int k=0;
    while(1)
    {
        for(k=0;k<=8;k++){
            mRedON;
//            strum();
            mWaitms(50);
            mYellowON;
        }
        mRedOFF;
//        damp();
        mWaitms(1000);
    }
    return(0);
}


//void picker_setup(void){
//    /*Picket between 2000, and 6600
//     Arm between 1500 and 4000
//     Arm Picks between 2800 and 3200
//     */
//    set_gpio(GPIOA,GPIO_Pin_4,MODE_ALT_FUNCTION); //Step Pulse (Mode 3 for pwm)
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_10);//Connect Pin to Timer
//    set_gpio(GPIOA,GPIO_Pin_5,MODE_ALT_FUNCTION); //Step Pulse (Mode 3 for pwm)
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_10);//Connect Pin to Timer
//    set_pwm(60000,2000,24);
//}
//void stepper_setup(void){
//    set_gpio(GPIOA,GPIO_Pin_8,MODE_OUT); //Mode 2 Pin
//    set_gpio(GPIOA,GPIO_Pin_9,MODE_OUT); //Mode 1 Pin
//    set_gpio(GPIOA,GPIO_Pin_10,MODE_OUT); //Mode 0 Pin
//    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//    GPIO_ResetBits(GPIOA,GPIO_Pin_9);
//    GPIO_ResetBits(GPIOA,GPIO_Pin_10);
//    
//    set_gpio(GPIOB,GPIO_Pin_4,MODE_OUT); //Mode 1 Pin
//    set_gpio(GPIOB,GPIO_Pin_3,MODE_ALT_FUNCTION); //Mode 0 Pin
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1);//Connect Pin to Timer
//    /*ON TIMER 2 CHANNEL 2*/
//    /*Have to make a more general PWM Function Creator*/
//    
//}
//void strum(void){
//    
//    switch(pose){
//        case 1:
//            modulatePick(4400);
//            modulateArm(2700);
//            pose=2;
//            break;
//        case 2:
//            modulatePick(5400);
//            modulateArm(3100);
//            pose=1;
//            break;
//        default:
//            mWhiteON;
//            mWaitms(100);
//            mWhiteOFF;
//            break;
//    }
//}
//void damp(void){
//    modulatePick(4900);
//    modulateArm(2870);
//}
//
//void TIM12_IRQHandler(void)
//{
//    
//    TIM_ClearFlag(TIM12,TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4);
//}
