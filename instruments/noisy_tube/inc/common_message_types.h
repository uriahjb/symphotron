#ifndef COMMON_MESSAGE_TYPES_H
#define COMMON_MESSAGE_TYPES_H

#include <stdint.h>

// Midi Messages

/// DRAFT: is this the useful metadata?
/// Radio metadata message wraper for sending messages over the air.
/// * Don't delare a struct with this, just use it to pack into and extract from 
///   byte arrays, in which case the data field length won't matter.
/*
const uint8_t kTypeRadio = 0x01;
typedef struct __attribute__ ((__packed__)) {
  uint8_t receiver_id;      /// node ID of recipient
  uint8_t reply_to_channel; /// any reply should go on this channel
  uint8_t reply_to_id;      /// any reply should go to this ID
  uint8_t type;               /// sent message type
  uint8_t data;               /// first byte of message data
} MsgRadio;
*/

const uint8_t MsgTypeMidi = 0x01;
typedef struct __attribute__ ((__packed__)) {
  uint8_t channel;
  uint8_t status;
  uint8_t data[2];
} MsgMidi;

#endif // COMMON_MESSAGE_TYPES_H
