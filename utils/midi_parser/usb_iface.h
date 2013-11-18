/*
  Simple usb_interface, puts bytes in byte in-queue and
  writes bytes from out-queue
*/
// Extern C since this is pure C
extern "C" {
#include "mUSB.h"
}

#include "bqueue.h"

#define MAX_PRINT_LEN 64

class usbIface {
public:
  usbIface( bQueue* in_queue, bQueue* out_queue );
  
  // Read/Write bytes from queues over usb
  void readBytes( void );
  void writeBytes( void );

  // printf utility function 
  int8_t printf( char* fmt, ... );

private:
  bQueue* m_in;
  bQueue* m_out; 
  char m_print_str[MAX_PRINT_LEN]; 
};
 
  
