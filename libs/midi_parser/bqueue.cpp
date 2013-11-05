/*
  Simple queue implementation
*/
#include "bqueue.h"

#define QUEUE_FULL 0
#define QUEUE_EMPTY 1

bQueue::bQueue( void ) {
  start_ind = 0;
  end_ind = 0;
  byte_cnt = 0;
}

uint8_t bQueue::isempty( void ) {
  if ( byte_cnt > 0 ) 
    return true;
  return false;
}

uint8_t bQueue::enqueue( uint8_t byte ) {
  // Increment end index 
  end_ind++;
  // Check if end index is beyond bounds, if so wraparound
  if ( end_ind > QUEUE_SIZE )
    end_ind = 0;
  // Check if there is a collision between start index and end index 
  if ( start_ind == end_ind )
    return QUEUE_FULL;
  // Otherwise increment byte_cnt and add byte to buffer
  byte_cnt++;
  buf[end_ind] = byte;
  return QUEUE_SUCCESS;
}   

uint8_t bQueue::dequeue( uint8_t* byte ) { 
  // Check if queue is empty  
  if ( start_ind == end_ind )
    return QUEUE_EMPTY;
  // Increment start index 
  start_ind++;
  // Check if index is beyond bounds, if so wraparound
  if ( start_ind > QUEUE_SIZE )
    start_ind = 0;
  // Decrement byte count
  byte_cnt--;
  *byte = buf[start_ind];
  return QUEUE_SUCCESS;
}
     

   
