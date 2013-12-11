/** 
** This template will guide you in making a ST M4 project.  
** All locations with double comments require your attention (** or ///)
** Please perform the following tasks:
**
** Edit peripherals.h to enable the desired peripherals
** Edit peripherals.c to enable the desired peripherals
** Include custom headers
** Write your code in the specified areas
** Comment your code
** Delete this comment and put a description of your project and the used hardware
**/

#include "stm32f37x.h"
#include "mGeneral.h"
//#include "mEasy.h"
#include "common_peripherals.h"
#include "common_message_types.h"
// COMMON_MESSAGE_TYPES_H"
/// Include your custom headers here
// #include "usb_interface.h"
#include "zigbee_interface.h"
#include "timer_1.h"
#define debug 1
// usb for debugging
#include "bqueue.h"
#include "usb_iface.h"

// For usb debugging
#include "midi_parser.h"


int GPIO_INI(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct_1;    //initialise GPIO
    GPIO_InitStruct_1.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;   //Configure pin B3,4,5,6,7,8,9 for GPIO
    GPIO_InitStruct_1.GPIO_Mode = GPIO_Mode_OUT;    //configure pins as output
    GPIO_InitStruct_1.GPIO_Speed = GPIO_Speed_50MHz;    //Medium mode (2MHz)
    GPIO_InitStruct_1.GPIO_OType = GPIO_OType_PP;   //output type as push pull
    GPIO_InitStruct_1.GPIO_PuPd = GPIO_PuPd_NOPULL;   //no pull down or pull up resistor
    GPIO_Init(GPIOB, &GPIO_InitStruct_1);
    
    GPIO_InitTypeDef  GPIO_InitStruct_2;    //initialise GPIO
    GPIO_InitStruct_2.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;   //Configure pin F6 and F7 for GPIO
    GPIO_InitStruct_2.GPIO_Mode = GPIO_Mode_OUT;    //configure pins as output
    GPIO_InitStruct_2.GPIO_Speed = GPIO_Speed_50MHz;    //Medium mode (2MHz)
    GPIO_InitStruct_2.GPIO_OType = GPIO_OType_PP;   //output type as push pull
    GPIO_InitStruct_2.GPIO_PuPd = GPIO_PuPd_NOPULL;   //no pull down or pull up resistor
    GPIO_Init(GPIOF, &GPIO_InitStruct_2);
}
int Play_note_A(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    GPIO_ResetBits(GPIOB , GPIO_Pin_4);
    GPIO_SetBits(GPIOB , GPIO_Pin_3);
    mRedON;
}

int Stop_note_A(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    GPIO_SetBits(GPIOB , GPIO_Pin_4);
    GPIO_ResetBits(GPIOB , GPIO_Pin_3);
    //mWaitms(500);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    mRedOFF;
}

int Play_note_B(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_7);
    GPIO_SetBits(GPIOB , GPIO_Pin_9);
    GPIO_ResetBits(GPIOB , GPIO_Pin_5);
    mYellowON;
}

int Stop_note_B(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_7);
    GPIO_ResetBits(GPIOB , GPIO_Pin_9);
    GPIO_SetBits(GPIOB , GPIO_Pin_5);
    //mWaitms(500);
    GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    mYellowOFF;
}
int Play_note_C(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_8);
    GPIO_SetBits(GPIOF , GPIO_Pin_6);
    GPIO_ResetBits(GPIOF , GPIO_Pin_7);
    mGreenON;
}

int Stop_note_C(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_8);
    GPIO_ResetBits(GPIOF , GPIO_Pin_6);
    GPIO_SetBits(GPIOF , GPIO_Pin_7);
    //mWaitms(500);
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    mGreenOFF;
}



// main function
int main(void)
{ 
    mInit();
    mUSBInit();
    GPIO_INI();
    
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
    
   while (1)
   {
      
      usbIfaceReadBytes(&usb);
     
      // Handle midi message from usb
      if(midiParserHasMsg(&parser,&msg)){
          usbIfacePrintf(&usb, "status  %02x\n",msg.status);
          usbIfacePrintf(&usb, "channel %02x\n",msg.channel);
          //usbIfacePrintf(&usb, "data[0] %02x\n",msg.data[0]);
          usbIfacePrintf(&usb, "data[0] %d\n",msg.data[0]);
          usbIfacePrintf(&usb, "data[1] %02x\n\n",msg.data[1]);
          mWhiteTOGGLE;
          if ( msg.status == NOTE_ON ) {
              switch ( msg.data[0] ) {
                  // Note A Octave -1
                  case 57:
                      Play_note_A();
                      break;
                  // Note A# Octave -1
                  case 58:
                      Play_note_B();
                      break;
                  // Note C Octave 0
                  case 60:
                      Play_note_C();
                      break;
              }
          }
          
          if ( msg.status == NOTE_OFF ) {
              switch ( msg.data[0] ) {
                      // Note A Octave -1
                  case 57:
                      Stop_note_A();
                      break;
                      // Note A# Octave -1
                  case 58:
                      Stop_note_B();
                      break;
                      // Note C Octave 0
                  case 60:
                      Stop_note_C();
                      break;
              }
          }
       
          
      }
       
      usbIfaceWriteBytes(&usb);

      /*
      Play_note_A();
      mWaitms(1000);
      Stop_note_A();
      mWaitms(1000);
       
      Play_note_B();
      mWaitms(1000);
      Stop_note_B();
      mWaitms(1000);
      Play_note_C();
      mWaitms(1000);
      Stop_note_C();
      mWaitms(1000);
      */
   }

 
}
