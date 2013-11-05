/*
  Simple queue implementation
*/
#include "stdint.h"

#define QUEUE_SIZE 100

#define QUEUE_FULL 0
#define QUEUE_EMPTY 1
#define QUEUE_SUCCESS 2

class bQueue {
public:
  bQueue( void );
  uint8_t isempty( void );
  uint8_t dequeue( uint8_t* byte );
  uint8_t enqueue( uint8_t byte );
private:
  uint16_t start_ind;
  uint16_t end_ind;
  uint8_t byte_cnt;
  uint8_t buf[QUEUE_SIZE];
};

