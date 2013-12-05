/*
  Simple queue implementation
*/
#include "bqueue.h"

#define QUEUE_FULL 0
#define QUEUE_EMPTY 1

void bQueueInit( bQueue *q ) { 
  q->start_ind = 0;
  q->end_ind = 0;
  q->byte_cnt = 0;
}

uint8_t bQueueIsEmpty( bQueue *q ) {
  if ( q->byte_cnt > 0 ) 
    return 0;
  return 1;
}

uint8_t bQueueBytesAvailable( bQueue *q ) {
  return q->byte_cnt;
}

uint8_t bQueueEnqueue( bQueue *q, uint8_t byte ) {
  // Increment end index 
  q->end_ind++;
  // Check if end index is beyond bounds, if so wraparound
  if ( q->end_ind > QUEUE_SIZE )
    q->end_ind = 0;
  // Check if there is a collision between start index and end index 
  if ( q->start_ind == q->end_ind )
    return QUEUE_FULL;
  // Otherwise increment byte_cnt and add byte to buffer
  q->byte_cnt++;
  q->buf[q->end_ind] = byte;
  return QUEUE_SUCCESS;
}   

uint8_t bQueueDequeue( bQueue *q, uint8_t* byte ) { 
  // Check if queue is empty  
  if ( q->start_ind == q->end_ind )
    return QUEUE_EMPTY;
  // Increment start index 
  q->start_ind++;
  // Check if index is beyond bounds, if so wraparound
  if ( q->start_ind > QUEUE_SIZE )
    q->start_ind = 0;
  // Decrement byte count
  q->byte_cnt--;
  *byte = q->buf[q->start_ind];
  return QUEUE_SUCCESS;
}

/*
bQueue::bQueue( void ) {
  start_ind = 0;
  end_ind = 0;
  byte_cnt = 0;
}

uint8_t bQueue::isempty( void ) {
  if ( byte_cnt > 0 ) 
    return false;
  return true;
}

uint8_t bQueue::bytesAvailable( void ) {
  return byte_cnt;
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
*/

   
