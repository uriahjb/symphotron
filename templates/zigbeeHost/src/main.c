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
** Used the custom zigbee board for the M4 to send data
** reading data in from USART
** had the modify SPI pins for the M4 which are different from those in the 
** PPR board
**/


/* 
  M4 Midi Host Code:
    Reads in midi messages over usb and forwards them over zigbee
*/

#include "stm32f37x.h"
#include "mGeneral.h"
#include "common_peripherals.h"
#include "My_Usart.h"
#include "common_message_types.h"
/// Include your custom headers here
// #include "usb_interface.h"
#include "zigbee_interface.h"

// Midi and usb 
#include "bqueue.h"
#include "usb_iface.h" 
#include "midi_parser.h"

// main function
int main(void)
{ 
  mInit();
  mUSBInit();

  // Set up usb interface
  bQueue in_q;
  bQueue out_q;
  bQueueInit( &in_q );
  bQueueInit( &out_q );
  usbIface usb;
  usbIfaceInit( &usb, &in_q, &out_q );

  midiMsg input_msg;
  midiParser midi;
  midiParserInit( &midi, &in_q );

  // Confusing naming, zigbee midi message 
  MsgMidi output_msg; 

  //USART_Config();
  mWhiteON;
  InitPeripherals();  // configure GPIO, USB, I2C, ect based on peripherals.h
  
  /// Initialize code here
  // InitUsbInterface();
  // initialize Zigbee interface object
  struct ZigbeeInterface zigbee_interface;
  InitZigbeeInterface(&zigbee_interface); //Init Zigbee interface
  // mBlueON;
  //MsgRotmsg rotations_out;  //make a variable for the struct defined in common_message_types
  //uint8_t usart_data = 0;
  // main loop
  while(1)
  {
    // Read data in from usb rx buffer
    usbIfaceReadBytes( &usb );

    // Check if we have a new midi message
    if ( midiParserHasMsg(&midi, &input_msg) ) {
      mGreenTOGGLE;
      // If so send a midi message over zigbee
      output_msg.status  = input_msg.status;
      output_msg.channel = input_msg.channel;
      output_msg.data[0] = input_msg.data[0];
      output_msg.data[1] = input_msg.data[1];

      // When we get a message print it back over usb for debugging
      usbIfacePrintf(&usb, "status  %02x\n",output_msg.status);
      usbIfacePrintf(&usb, "channel %02x\n",output_msg.channel);
      usbIfacePrintf(&usb, "data[0] %02x\n",output_msg.data[0]);
      usbIfacePrintf(&usb, "data[1] %02x\n\n",output_msg.data[1]);

      SendZigbeePacket(&zigbee_interface, MsgTypeMidi, (uint8_t*)&output_msg, sizeof(MsgMidi)); //Send the packet
    }
    // Write data out of usb tx buffer 
    usbIfaceWriteBytes( &usb );
    
    // Send out bytes in zigbee tx buffer
    SendZigbeeNow(&zigbee_interface);
  }
  
  return(0);  
}
