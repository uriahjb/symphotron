// These are C things extern C so we can mix C and CPP
extern "C" {
#include "mGeneral.h"
#include "mBus.h"
#include "mUSB.h"
}

#include "bqueue.h"
#include "usb_iface.h" 
#include "midi_parser.h"

int main( void )
{
    // Initialize m4 things
    mInit();
    //mBusInit();
    mUSBInit();

    // Set up usb interface
    bQueue in_q;
    bQueue out_q;
    usbIface usb( &in_q, &out_q );
    
    // Initialize midi interface
    midiParser midi( &in_q );
    midiMsg msg;

    mYellowON;
    
    while(1)
    {
      usb.readBytes();
      /*
      mWaitms(500);
      mGreenON;
      usb.printf( "hello world\n" );
      mWaitms(500);
      mGreenOFF;
      */
      if ( midi.hasMsg(&msg) ) {
        mGreenTOGGLE;
        usb.printf("msg: %02x\n", msg.status);
        usb.printf("channel: %02x\n", msg.channel);
        usb.printf("data[0]: %02x\n", msg.data[0]);
        usb.printf("data[1]: %02x\n", msg.data[1]);
      }
      // Write data out 
      usb.writeBytes();
    }
}
