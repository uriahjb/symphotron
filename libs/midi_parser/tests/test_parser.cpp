/*
  Parser test 
*/
#include "stdio.h"
#include "bqueue.h"
#include "midi_parser.h"

int main( void ) {
  bQueue bq;
  midiParser midi( &bq );
  midiMsg msg;
  
  uint8_t res;
  // Enqueue note off message
  res = bq.enqueue( 0x80 );
  if ( res == QUEUE_FULL )
    printf( "queue full\n" );
  res = bq.enqueue( 0x0A );
  if ( res == QUEUE_FULL )
    printf( "queue full\n" );
  res = bq.enqueue( 0x0B );
  if ( res == QUEUE_FULL )
    printf( "queue full\n" );

  // Enqueue real-time message
  res = bq.enqueue( 0xF8 );
  if ( res == QUEUE_FULL )
    printf( "queue full\n" );
  
  // Enqueue note on message
  res = bq.enqueue( 0x90 );
  if ( res == QUEUE_FULL )
    printf( "queue full\n" );
  res = bq.enqueue( 0x01 );
  if ( res == QUEUE_FULL )
    printf( "queue full\n" );
  res = bq.enqueue( 0x02 );
  if ( res == QUEUE_FULL )
    printf( "queue full\n" );
  printf(" bAvailable: %02x\n", bq.bytesAvailable());

  while ( !bq.isempty() ) {
    if ( midi.hasMsg( &msg ) ) {
      printf("msg: \n"
             "\tstatus: %02x\n"
             "\tchannel: %02x\n"
             "\tdata[0]: %02x\n"
             "\tdata[1]: %02x\n", msg.status, msg.channel, msg.data[0], msg.data[1]);
    }
  }
               
  printf( "complete\n" );
}
