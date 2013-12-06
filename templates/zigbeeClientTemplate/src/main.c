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
#include "common_peripherals.h"
#include "common_message_types.h"
// COMMON_MESSAGE_TYPES_H"
/// Include your custom headers here
// #include "usb_interface.h"
#include "zigbee_interface.h"
#define debug 1

// usb for debugging
#include "bqueue.h"
#include "usb_iface.h" 

// main function
int main(void)
{ 
  mInit();
  mUSBInit();
  InitPeripherals();  // configure GPIO, USB, I2C, ect based on peripherals.h

  // Set up usb interface
  bQueue in_q;
  bQueue out_q;
  bQueueInit( &in_q );
  bQueueInit( &out_q );
  usbIface usb;
  usbIfaceInit( &usb, &in_q, &out_q );

  mWhiteON;
  // Initialize Zigbee interface object
  struct ZigbeeInterface zigbee_interface;
  InitZigbeeInterface(&zigbee_interface);
  ZigBee_SetModeRx();
  
  uint8_t *received_zigbee_data; // temporary pointer to received type+data bytes
  uint8_t received_zigbee_length; // number of of received type+data bytes

  while(1)
  {
    // Read bytes in from zigbee
    GetZigbeeBytes(&zigbee_interface);

    if ( PeekZigbeePacket(&zigbee_interface, &received_zigbee_data, &received_zigbee_length) ) {
      mGreenTOGGLE;
      // All message types should just be the midi message 
      uint8_t type = received_zigbee_data[0];
      MsgMidi* msg = (MsgMidi*)(received_zigbee_data+1);
      
      // When we get a message print it back over usb for debugging
      usbIfacePrintf(&usb, "status  %02x\n",msg->status);
      usbIfacePrintf(&usb, "channel %02x\n",msg->channel);
      usbIfacePrintf(&usb, "data[0] %02x\n",msg->data[0]);
      usbIfacePrintf(&usb, "data[1] %02x\n\n",msg->data[1]);

      // Do some things like set desired note, etc ...
      DropZigbeePacket(&zigbee_interface);
    }

    // Write data out of usb tx buffer 
    usbIfaceWriteBytes( &usb );
  }
  
  return(0);
}
