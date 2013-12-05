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
    mBusInit();
    
    
    bQueue in_q;
    bQueue out_q;
    bQueueInit( &in_q );
    bQueueInit( &out_q );
    usbIface usb;
    usbIfaceInit( &usb, &in_q, &out_q );
    
    // Set up midi parser
    midiParser parcer;
    midiParserInit( &parcer, &in_q );
    midiMsg msg;
    
    picker_setup();
    stepper_setup();
    set_timer5 (60000,30000,1200);
    
    setMode( STEP_4TH);
    setSpeedLimits( 65, 200 );
    set_speed(m_speed_min);
    setAccelerationLimit(1500);
    
    int test=0;
    
    //    uint16_t pos[]={0,-180,0,-360,0,-540,0,-720,-900,0};
    //    uint16_t dur[]={1000,1000,1000,1000};
    uint16_t pos=0;
    set_speed(m_speed_min);
    int i=0;
    update(0);
    //    TIM_Cmd(TIM5, DISABLE);
    int Sflag=0;
    accStop();
    
    
    strum_delay(1);
    while(1)
    {
        //        test = update(pos);
        //        if(test==1){
        //            if(Sflag==1){
        //                strum();
        //                Sflag=0;
        //            }
        //        }
        
        
        usbIfaceReadBytes(&usb);
        if(midiParserHasMsg(&parcer,&msg)){
            mGreenTOGGLE;
            usbIfacePrintf(&usb, "status  %02x\n",msg.status);
            usbIfacePrintf(&usb, "channel %02x\n",msg.channel);
            usbIfacePrintf(&usb, "data[0] %02x\n",msg.data[0]);
            usbIfacePrintf(&usb, "data[1] %02x\n\n",msg.data[1]);
            if(msg.status==NOTE_ON){
                //                strum();
                Sflag=1;
                i++;
                if(i%2==0){
                    set_direction(1);
                }else{
                    set_direction(0);
                }
                switch(msg.data[0]){
                    case 52:
                        set_freq(165);
                        break;
                    case 53:
                        set_freq(174);
                        break;
                    case 54:
                        set_freq(185);
                        break;
                    case 55:
                        set_freq(196);
                        break;
                    case 56:
                        set_freq(207);
                        break;
                    case 57:
                        //                        pos=A3;
                        set_freq(220);
                        break;
                    case 58 :
                        set_freq(233);
                        break;
                    case 59:
                        //                        pos=B3;
                        set_freq(246);
                        break;
                    case 60:
                        //                        pos=C4;
                        set_freq(261);
                        break;
                    case 61:
                        set_freq(277);
                        break;
                    case 62:
                        //                        pos=D4;
                        set_freq(294);
                        break;
                    case 63 :
                        set_freq(311);
                        break;
                    case 64:
                        //                        pos=E4;
                        set_freq(330);
                        break;
                    case 65:
                        //                        pos=F4;
                        set_freq(350);
                        break;
                    case 67:
                        //                        pos=G4;
                        set_freq(392);
                        break;
                    case 68:
                        set_freq(415);
                        break;
                    case 69:
                        //                        pos=A4;
                        set_freq(440);
                        break;
                    case 70:
                        set_freq(466);
                        break;
                    case 71:
                        //                        pos=B4;
                        set_freq(493);
                        break;
                    case 72:
                        //                        pos=C5;
                        set_freq(523);
                        break;
                    case 73:
                        set_freq(554);
                        break;
                    case 74:
                        set_freq(587);
                        break;
                    case 75:
                        set_freq(622);
                        break;
                    case 76:
                        set_freq(660);
                        break;
                    case 77:
                        set_freq(698);
                        break;
                    case 78:
                        set_freq(740);
                        break;
                    case 79:
                        set_freq(783);
                        break;
                    case 80:
                        set_freq(830);
                        break;
                    case 81:
                        set_freq(880);
                        break;
                        
                        
                        
                        
                        
                        
                        
//                    case 57:
//                        pos=A3;
//                        break;
//                    case 59:
//                        pos=B3;
//                        break;
//                    case 60:
//                        pos=C4;
//                        break;
//                    case 62:
//                        pos=D4;
//                        break;
//                    case 64:
//                        pos=E4;
//                        break;
//                        pos=F4;
//                        break;
//                    case 67:
//                        pos=G4;
//                        break;
//                    case 69:
//                        pos=A4;
//                        break;
//                    case 71:
//                        pos=B4;
//                        break;
//                    case 72:
//                        pos=C5;
//                        break;
                    default:
                        break;
                }
            }
        }
        usbIfaceWriteBytes(&usb);
        
    }
    
    //        test = update(pos[test2]);
    //        if(test==1){
    //             mRedOFF;
    //            strum_speed(1-i/11);
    //            test2++;
    //            i++;
    //
    ////            mWaitms(100);
    //        }
    //        if(test2>=11){
    //            test2=11;
    //            mWhiteON;
    
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
