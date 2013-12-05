/*
  Simple usb interface
*/
#include "usb_iface.h"
#include "stdarg.h"
#include "stdio.h"

void usbIfaceInit( usbIface *iface, bQueue* in_queue, bQueue* out_queue ) {
  iface->m_in = in_queue;
  iface->m_out = out_queue;
}

void usbIfaceReadBytes( usbIface *iface ) {
  CDC_Receive_DATA();
  for ( uint8_t i=0; i < Receive_length; i++ ) 
    bQueueEnqueue( iface->m_in, Receive_Buffer[i] ); // Not checking for errors
  Receive_length = 0;
}

void usbIfaceWriteBytes( usbIface *iface ) {
  uint8_t byte;
  uint8_t buf[64];
  uint8_t cnt = 0;
  if(!(GetEPTxStatus(ENDP1)==EP_TX_VALID)) {
    while ( !bQueueIsEmpty( iface->m_out ) && cnt < 64 ) {
      bQueueDequeue( iface->m_out, &byte );
      buf[cnt] = byte;
      cnt++;
    }
    CDC_Send_DATA(buf, cnt);
  }
}

int8_t usbIfacePrintf( usbIface *iface, char* fmt, ... ) {
    uint16_t len;
    va_list args;
    va_start( args, fmt );
    vsprintf( iface->m_print_str, fmt, args );
    va_end( args );
     
    len = strlen(iface->m_print_str);
    //bQueueEnqueue( iface->m_out, len );

    if ( len > MAX_PRINT_LEN ) {
        return 0;
    }
   
    /* 
    for ( uint8_t i=0; i<len; i++ ) {
      //bQueueEnqueue( iface->m_out, iface->m_print_str[i] );
      bQueueEnqueue( iface->m_out, 10 );
    }
    */
   
    /* 
    for ( uint8_t i=0; i<6; i++ ) {
      bQueueEnqueue( iface->m_out, i );
    }
    */
    
    /* 
    bQueueEnqueue( iface->m_out, 'h' );
    bQueueEnqueue( iface->m_out, 'e' );
    bQueueEnqueue( iface->m_out, 'l' );
    bQueueEnqueue( iface->m_out, 'l' );
    bQueueEnqueue( iface->m_out, 'o' );
    */
    bQueueEnqueue( iface->m_out, 1 );
    bQueueEnqueue( iface->m_out, '\n' );
    
    return 1; 
}

/*
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
*/
