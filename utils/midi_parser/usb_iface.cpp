/*
  Simple usb interface
*/
#include "usb_iface.h"
#include "stdarg.h"
#include "stdio.h"

usbIface::usbIface( bQueue* in_queue, bQueue* out_queue ) {
  m_in = in_queue;
  m_out = out_queue;
}

void usbIface::readBytes( void ) {
  CDC_Receive_DATA();
  for ( uint8_t i=0; i < Receive_length; i++ ) 
    m_in->enqueue( Receive_Buffer[i] ); // Not checking for errors
  Receive_length = 0;
}

void usbIface::writeBytes( void ) {
  uint8_t byte;
  uint8_t buf[64];
  uint8_t cnt = 0;
  if(!(GetEPTxStatus(ENDP1)==EP_TX_VALID)) {
    while ( !m_out->isempty() && cnt < 64 ) {
      m_out->dequeue( &byte );
      buf[cnt] = byte;
      cnt++;
    }
    CDC_Send_DATA(buf, cnt);
  }
  /*
  if(!(GetEPTxStatus(ENDP1)==EP_TX_VALID) && !m_out->isempty()) {    
    m_out->dequeue( &byte ); // Not checking for errors this is bad
    CDC_Send_DATA(&byte, 1);
  }
  */
}

int8_t usbIface::printf( char* fmt, ... ) {
    uint16_t len;
    va_list args;
    va_start( args, fmt );
    vsprintf( m_print_str, fmt, args );
    va_end( args );
     
    len = strlen(m_print_str);
    if ( len > MAX_PRINT_LEN ) {
        return 0;
    }
    for ( uint8_t i=0; i<len; i++ )
      m_out->enqueue( m_print_str[i] );
    return 1; 
}
