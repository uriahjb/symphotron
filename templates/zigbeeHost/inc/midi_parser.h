/*
  Lightweight Midi Parser for embedded systems
*/

/*
  Status byte definitions from:
    http://www.midi.org/techspecs/midimessages.php
*/

#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H

#include "bqueue.h"

// Channel voice messages
#define NOTE_OFF 0x80
#define NOTE_ON  0x90
#define POLYPHONIC_AFTERTOUCH 0xA0
#define CONTROL_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0
#define CHANNEL_PRESSURE 0xD0
#define PITCH_WHEEL 0xE0

// System common messages
#define SYSTEM_EXCLUSIVE 0xF0
#define TIME_QTR_FRAME 0xF1
#define SONG_POS_PTR 0xF2
#define SONG_SELECT 0xF3
#define TUNE_REQ 0xF6
#define EOX 0xF7

// System Real-Time messages
#define TIMING_CLOCK 0xF8
#define START 0xFA
#define CONTINUE 0xFB
#define STOP 0xFC
#define ACTIVE_SENSING 0xFE
#define SYSTEM_RESET 0xFF

// Broadcast channel, these messages are for everyone
#define BROADCAST 0xFF

typedef struct {
  uint8_t channel;
  uint8_t status;
  uint8_t data[2];
} midiMsg;

typedef struct {
  // Pointer to the byte queue we are reading from 
  bQueue *m_bq;
  // Two local msg types channel/common msg and real-time msg 
  midiMsg m_msg;  
  midiMsg m_rt_msg;
  uint8_t m_data_ind;
  uint8_t expected_bytes; 
} midiParser;

void midiParserInit( midiParser *parser, bQueue* bq );
uint8_t midiParserHasMsg( midiParser *parser, midiMsg* msg );

/*
class midiParser {
public:
  midiParser( bQueue* bq );
  uint8_t hasMsg( midiMsg* msg );
    
private:
  // Pointer to the byte queue we are reading from 
  bQueue *m_bq;
  // Two local msg types channel/common msg and real-time msg 
  midiMsg m_msg;  
  midiMsg m_rt_msg;
  uint8_t m_data_ind;
  uint8_t expected_bytes; 
  
};
*/

#endif 
