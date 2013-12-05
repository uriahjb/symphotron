/*
  Simple queue implementation
*/
#ifndef BQUEUE_H
#define BQUEUE_H

#include "stdint.h"

#define QUEUE_SIZE 100

#define QUEUE_FULL 0
#define QUEUE_EMPTY 1
#define QUEUE_SUCCESS 2

typedef struct { 
  uint16_t start_ind;
  uint16_t end_ind;
  uint8_t byte_cnt;
  uint8_t buf[QUEUE_SIZE];
} bQueue;

void bQueueInit( bQueue *q );
uint8_t bQueueIsEmpty( bQueue *q );
uint8_t bQueueBytesAvailable( bQueue *q );
uint8_t bQueueDequeue( bQueue *q, uint8_t* byte );
uint8_t bQueueEnqueue( bQueue *q, uint8_t byte );

/*
class bQueue {
public:
  bQueue( void );
  uint8_t isempty( void );
  uint8_t bytesAvailable( void );
  uint8_t dequeue( uint8_t* byte );
  uint8_t enqueue( uint8_t byte );
private:
  uint16_t start_ind;
  uint16_t end_ind;
  uint8_t byte_cnt;
  uint8_t buf[QUEUE_SIZE];
};
*/

#endif 
