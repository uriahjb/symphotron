/*
  bQueue test 
*/
#include "stdio.h"
#include "bqueue.h"

int main( void ) {
  bQueue bq;
  uint8_t res;
  uint8_t byte;

  for ( uint8_t i = 0; i < QUEUE_SIZE/2; i++ ) {
    res = bq.enqueue( i );
    if ( res == QUEUE_FULL )
      printf( "queue full\n" );
  }
  for ( uint8_t i = 0; i < QUEUE_SIZE/2; i++ ) {
    res = bq.dequeue( &byte );
    printf( "byte: %d \n", byte );
    if ( res == QUEUE_EMPTY)
      printf( "queue empty\n" );
  }
  for ( uint8_t i = 0; i < QUEUE_SIZE; i++ ) {
    res = bq.enqueue( i );
    if ( res == QUEUE_FULL )
      printf( "queue full\n" );
  }
  for ( uint8_t i = 0; i < QUEUE_SIZE; i++ ) {
    res = bq.dequeue( &byte );
    printf( "byte: %d \n", byte );
    if ( res == QUEUE_EMPTY)
      printf( "queue empty\n" );
  }
  for ( uint8_t i = 0; i < QUEUE_SIZE; i++ ) {
    res = bq.enqueue( i );
    if ( res == QUEUE_FULL )
      printf( "queue full\n" );
  }
  for ( uint8_t i = 0; i < QUEUE_SIZE; i++ ) {
    res = bq.dequeue( &byte );
    printf( "byte: %d \n", byte );
    if ( res == QUEUE_EMPTY)
      printf( "queue empty\n" );
  }

  printf( "complete\n" );
}
