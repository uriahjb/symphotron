#include "mGeneral.h"
#include "mBus.h"
#include "mUSB.h"
#include "stdio.h"
#include "stm32f37x.h"
#include "mEasy.h"
#include "stepper.h"
#include <math.h>
#include "arm_math.h"
#include "Stepper.h"

#include "bqueue.h"
#include "usb_iface.h"
#include "midi_parser.h"

// Mapping from note to stepper position
#define G3  -190
#define A3  -370
#define B3  -550
#define C4  -660
#define D4  -780
#define E4  -900
#define F4  -980
#define G4  -1070
#define A4  -1160
#define B4  -1260
#define C5  -1310

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

void strum_delay(float speed){
    TIM5->ARR=60000*speed;
    //        TIM5->CNT=0;
}


void set_freq(uint16_t Note){
    float Oct=1;
    if(Note<10)
        stop();
    else
        start();
    uint16_t T = (float)Oct*0.967*(1/(float)Note)*((72000000)/(float)Clock_Divide); //30 is the PWM clock divide
    TIM2->ARR=T;
    if(TIM2->ARR<TIM2->CNT)
        TIM2->CNT=0;
}

int main( void )
{
    mInit();
    mUSBInit();
    //mBusInit();
    
    bQueue in_q;
    bQueue out_q;
    bQueueInit( &in_q );
    bQueueInit( &out_q );
    usbIface usb;
    usbIfaceInit( &usb, &in_q, &out_q );
    
    // Set up midi parser
    midiParser parser;
    midiParserInit( &parser, &in_q );
    midiMsg msg;
    
    picker_setup();
    stepper_setup();
    set_timer5 (60000,30000,1200);
   
    setMode( STEP_8TH );
    setSpeedLimits( 65, 300 );
    set_speed(m_speed_min);
    setAccelerationLimit(500);
    
    int test=0;
    
    //    uint16_t pos[]={0,-180,0,-360,0,-540,0,-720,-900,0};
    //    uint16_t dur[]={1000,1000,1000,1000};
    
    uint16_t pos=0;
    /*
    //set_speed(m_speed_min);
    int i=0;
    update(0);
    //    TIM_Cmd(TIM5, DISABLE);
    accStop();
    setPosition(0);
    */
    int Sflag=0;
    
    
    //strum_delay(1);
    while(1)
    {
        //        test = update(pos);
        //        if(test==1){
        //            if(Sflag==1){
        //                strum();
        //                Sflag=0;
        //            }
        //        }
        update(pos);    
        
        usbIfaceReadBytes(&usb);
        if(midiParserHasMsg(&parser,&msg)){
            mGreenTOGGLE;
            usbIfacePrintf(&usb, "status  %02x\n",msg.status);
            usbIfacePrintf(&usb, "channel %02x\n",msg.channel);
            usbIfacePrintf(&usb, "data[0] %02x\n",msg.data[0]);
            usbIfacePrintf(&usb, "data[1] %02x\n\n",msg.data[1]);
            if(msg.status==NOTE_ON){
                strum();
                Sflag=1;
                switch(msg.data[0]){
                    case 57:
                        pos=A3;
                        break;
                    case 59:
                        pos=B3;
                        break;
                    case 60:
                        pos=C4;
                        break;
                    case 62:
                        pos=D4;
                        break;
                    case 64:
                        pos=E4;
                        break;
                        pos=F4;
                        break;
                    case 67:
                        pos=G4;
                        break;
                    case 69:
                        pos=A4;
                        break;
                    case 71:
                        pos=B4;
                        break;
                    case 72:
                        pos=C5;
                        break;
                    default:
                        break;
                }
            }
        }
        usbIfaceWriteBytes(&usb);
    }
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
            //modulatePick(3400);
            //modulateArm(2075);
            //modulatePick(3400);
            modulateArm(2800);
            pose=2;
            break;
        case 2:
            //modulatePick(4000);
            modulateArm(3400);
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
    //mRedON;
    //strum();
    TIM_ClearFlag(TIM5,TIM_FLAG_CC1);
    //TIM5->CNT=0;
}

void TIM12_IRQHandler(void)
{
    TIM_ClearFlag(TIM12,TIM_FLAG_CC1 | TIM_FLAG_CC2 | TIM_FLAG_CC3 | TIM_FLAG_CC4);
}
