/*
  Midi parser implementation:

    Parsing is a little funky since we can get real-time messages in 
    between other messages, basically we keep filling in channel/common
    msgs and if we run into a real-time message we just return that 
*/

#include "midi_parser.h"
#include "stdio.h"

//#define DEBUG

void midiParserInit( midiParser *parser, bQueue *bq ) {
  parser->m_bq = bq;
  parser->m_data_ind = 0;
}

uint8_t midiParserHasMsg( midiParser *parser, midiMsg *msg ) {
  uint8_t byte;
  uint8_t status;
  uint8_t channel;
  // If queue is empty then return no msg
  if ( bQueueDequeue( parser->m_bq, &byte ) == QUEUE_EMPTY )
    return 0;

  #ifdef DEBUG
    printf("byte: %02x \n", byte );
  #endif 
  // We have a new byte so lets do some parsing
  // Check if byte is status byte 
  if ( !(byte >> 7) ) {
    parser->m_msg.data[parser->m_data_ind] = byte;
    parser->m_data_ind++;
    #ifdef DEBUG
      printf("data_ind: %02x \n", parser->m_data_ind);
    #endif 
    if ( parser->m_data_ind == parser->expected_bytes) {
      // Reset index
      parser->m_data_ind = 0;
      // Copy message pointer
      *msg = parser->m_msg;
      #ifdef DEBUG
      printf("msg: \n"
             "\tstatus: %02x\n"
             "\tchannel: %02x\n"
             "\tdata[0]: %02x\n"
             "\tdata[1]: %02x\n", m_msg.status, m_msg.channel, m_msg.data[0], m_msg.data[1]);
      #endif
      return 1;
    }
    return 0;
  } 
  // Otherwise its a status byte
  // Check and see if its a realtime 
  status = byte & 0xF0;
  channel = byte & 0x0F;
  #ifdef DEBUG
    printf("status: %02x \n", status);
    printf("channel: %02x \n", channel);
  #endif 
  if ( byte > EOX ) {
    parser->m_rt_msg.status = byte;
    parser->m_rt_msg.channel = BROADCAST;
    *msg = parser->m_rt_msg;  
    return 1;
  }  
  // This is really messy 
  if ( byte < 0xF0 ) { 
    parser->m_msg.status = status;
    parser->m_msg.channel = channel;
    switch ( status ) {
    case NOTE_OFF:
      parser->expected_bytes = 2;
      break;
    case NOTE_ON:
      parser->expected_bytes = 2;
      break;
    case POLYPHONIC_AFTERTOUCH:
      parser->expected_bytes = 2;
      break;
    case CONTROL_CHANGE:
      parser->expected_bytes = 2;
      break;
    case PROGRAM_CHANGE:
      parser->expected_bytes = 1;
      break;
    case CHANNEL_PRESSURE:
      parser->expected_bytes = 1;
      break;
    case PITCH_WHEEL:
      parser->expected_bytes = 1;
      break;
    default:
      parser->expected_bytes = 0;
    }
  } else {
    parser->m_msg.status = byte;
    parser->m_msg.channel = BROADCAST;
    switch( byte ) {
    case TIME_QTR_FRAME:
      parser->expected_bytes = 1;
      break;
    case SONG_POS_PTR:
      parser->expected_bytes = 2;
      break;
    case SONG_SELECT:
      parser->expected_bytes = 1;
      break;
    default:
      parser->expected_bytes = 0;
      break; 
    }
  }
  if ( parser->expected_bytes == 0 ) {
    *msg = parser->m_msg;
    return 1;  
  }
  #ifdef DEBUG
    printf( "expected_bytes: %02x\n", expected_bytes );
  #endif
  return 0;
}

/*
midiParser::midiParser( bQueue* bq ) {
  m_bq = bq;
  m_data_ind = 0;
}

uint8_t midiParser::hasMsg( midiMsg* msg ) {
  uint8_t byte;
  uint8_t status;
  uint8_t channel;
  // If queue is empty then return no msg
  if ( m_bq->dequeue( &byte ) == QUEUE_EMPTY )    
    return 0;

  #ifdef DEBUG
    printf("byte: %02x \n", byte );
  #endif 
  // We have a new byte so lets do some parsing
  // Check if byte is status byte 
  if ( !(byte >> 7) ) {
    m_msg.data[m_data_ind] = byte;
    m_data_ind++;
    #ifdef DEBUG
      printf("data_ind: %02x \n", m_data_ind);
    #endif 
    if ( m_data_ind == expected_bytes) {
      // Reset index
      m_data_ind = 0;
      // Copy message pointer
      *msg = m_msg;
      #ifdef DEBUG
      printf("msg: \n"
             "\tstatus: %02x\n"
             "\tchannel: %02x\n"
             "\tdata[0]: %02x\n"
             "\tdata[1]: %02x\n", m_msg.status, m_msg.channel, m_msg.data[0], m_msg.data[1]);
      #endif
      return 1;
    }
    return 0;
  } 
  // Otherwise its a status byte
  // Check and see if its a realtime 
  status = byte & 0xF0;
  channel = byte & 0x0F;
  #ifdef DEBUG
    printf("status: %02x \n", status);
    printf("channel: %02x \n", channel);
  #endif 
  if ( byte > EOX ) {
    m_rt_msg.status = byte;
    m_rt_msg.channel = BROADCAST;
    *msg = m_rt_msg;  
    return 1;
  }  
  // This is really messy 
  if ( byte < 0xF0 ) { 
    m_msg.status = status;
    m_msg.channel = channel;
    switch ( status ) {
    case NOTE_OFF:
      expected_bytes = 2;
      break;
    case NOTE_ON:
      expected_bytes = 2;
      break;
    case POLYPHONIC_AFTERTOUCH:
      expected_bytes = 2;
      break;
    case CONTROL_CHANGE:
      expected_bytes = 2;
      break;
    case PROGRAM_CHANGE:
      expected_bytes = 1;
      break;
    case CHANNEL_PRESSURE:
      expected_bytes = 1;
      break;
    case PITCH_WHEEL:
      expected_bytes = 1;
      break;
    default:
      expected_bytes = 0;
    }
  } else {
    m_msg.status = byte;
    m_msg.channel = BROADCAST;
    switch( byte ) {
    case TIME_QTR_FRAME:
      expected_bytes = 1;
      break;
    case SONG_POS_PTR:
      expected_bytes = 2;
      break;
    case SONG_SELECT:
      expected_bytes = 1;
      break;
    default:
      expected_bytes = 0;
      break; 
    }
  }
  if ( expected_bytes == 0 ) {
    *msg = m_msg;
    return 1;  
  }
  #ifdef DEBUG
    printf( "expected_bytes: %02x\n", expected_bytes );
  #endif
  return 0;
}
*/
    
   
  

