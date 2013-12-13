// These are C things extern C so we can mix C and CPP
extern "C" {
#include "mGeneral.h"
#include "mBus.h"
#include "mUSB.h"
#include "stdio.h"
#include "stm32f37x.h"
#include "mEasy.h"
#include <math.h>
#include "arm_math.h"
#include "Stepper.h"
}

#include "bqueue.h"
#include "usb_iface.h" 

uint8_t a;
int8_t dir;
int8_t dir_cmd=1;
int pose=1;
int k=0;
uint32_t p=4000;
int test2=0;
int f=0;

void stepper_setup(void);
void strum(void);
void damp(void);

void hold(uint16_t T){ //Timer up to 1 seconds. (currently)
    
    TIM5->ARR=T*15000;
    TIM5->CNT=0;
    TIM_Cmd(TIM5, ENABLE);
}



int main( void )
{
    mInit();
    mUSBInit();
    mBusInit();

    // Set up usb interface
    bQueue in_q;
    bQueue out_q;
    usbIface usb( &in_q, &out_q );
    
//    picker_setup();
    stepper_setup();
//    set_timer5 (60000,30000,1200);
    
    setMode( STEP_8TH);
    setSpeedLimits( 65, 500 );
    set_speed(m_speed_min);
    setAccelerationLimit(100); //5000 is good
    
    uint8_t cmd; 
    uint16_t pos = 0;
    while(1)
    {
        usb.readBytes();

        // Handle bytes 
        if (!in_q.isempty() ) {
          mBlueTOGGLE;
          in_q.dequeue(&cmd);
          if ( cmd == 'l' ) {
            pos += 10;
          } else if ( cmd == 'h' ) {
            pos -= 10;
          }
        }
        update(pos);
    }
    return(0);
}

/*PWM ON TIM 12 PIN A4 CH 2*/
void TIM5_IRQHandler(void)
{
    test2++;
    TIM_ClearFlag(TIM5,TIM_FLAG_CC1);
    f=0;
}
