/*
  Simple usb_interface, puts bytes in byte in-queue and
  writes bytes from out-queue
*/
// Extern C since this is pure C

#ifndef USB_IFACE_H
#define USB_IFACE_H

#include "mUSB.h"

#include "bqueue.h"

#define MAX_PRINT_LEN 256

typedef struct {
  bQueue* m_in;
  bQueue* m_out; 
  char m_print_str[MAX_PRINT_LEN]; 
} usbIface;

void usbIfaceInit( usbIface *iface, bQueue* in_queue, bQueue* out_queue );
void usbIfaceReadBytes( usbIface *iface );
void usbIfaceWriteBytes( usbIface *iface );
int8_t usbIfacePrintf( usbIface *iface, char* fmt, ... );

/*
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
*/ 
 
#endif
