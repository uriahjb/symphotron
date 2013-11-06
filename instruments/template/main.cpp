// These are C things extern C so we can mix C and CPP
extern "C" {
#include "mGeneral.h"
#include "mBus.h"
#include "mUSB.h"
}

#include "bqueue.h"
#include "usb_iface.h" 

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

    mYellowON;
    
    while(1)
    {
        mWaitms(500);
        mGreenON;
        usb.printf( "hello world\n" );
        mWaitms(500);
        mGreenOFF;
        
        // Write data out 
        usb.writeBytes();
    }
}
