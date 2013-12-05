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

// main function
int main(void)
{ 
  mInit();
  USART_Config();
  InitPeripherals();  // configure GPIO, USB, I2C, ect based on peripherals.h

  mWhiteON;
  // Initialize Zigbee interface object
  struct ZigbeeInterface zigbee_interface;
  InitZigbeeInterface(&zigbee_interface);
  ZigBee_SetModeRx();
  
  uint8_t *received_zigbee_data; // temporary pointer to received type+data bytes
  uint8_t received_zigbee_length; // number of of received type+data bytes

  while(1)
  {

    GetZigbeeBytes(&zigbee_interface);
    if ( PeekZigbeePacket(&zigbee_interface, &received_zigbee_data, &received_zigbee_length) ) {
      mYellowTOGGLE;
      // All message types should just be the midi message 
      uint8_t type = received_zigbee_data[0];
      MsgMidi* msg = (MsgMidi*)(received_zigbee_data+1);

      // Do some things like set desired note, etc ...

      DropZigbeePacket(&zigbee_interface);
    }
  }
  
  return(0);
}
