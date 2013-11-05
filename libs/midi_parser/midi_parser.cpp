/*
  Midi parser implementation:

    Parsing is a little funky since we can get real-time messages in 
    between other messages, basically we keep filling in channel/common
    msgs and if we run into a real-time message we just return that 
*/

#include "midi_parser.h"
#include "stdio.h"

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

  printf("byte: %02x \n", byte );
  // We have a new byte so lets do some parsing
  // Check if byte is status byte 
  if ( !(byte >> 7) ) {
    m_msg.data[m_data_ind] = byte;
    m_data_ind++;
    printf("data_ind: %02x \n", m_data_ind);
    if ( m_data_ind == expected_bytes) {
      msg = &m_msg;
      printf("msg: \n"
             "\tstatus: %02x\n"
             "\tchannel: %02x\n"
             "\tdata[0]: %02x\n"
             "\tdata[1]: %02x\n", m_msg.status, m_msg.channel, m_msg.data[0], m_msg.data[1]);
      return 1;
    }
    return 0;
  } 
  // Otherwise its a status byte
  // Check and see if its a realtime 
  status = byte & 0xF0;
  channel = byte & 0x0F;
  printf("status: %02x \n", status);
  printf("channel: %02x \n", channel);
  if ( byte > EOX ) {
    m_rt_msg.status = status;
    m_rt_msg.channel = channel;
    msg = &m_rt_msg;  
    return 1;
  }  
  // If not realtime then just normal msg
  m_msg.status = status;
  m_msg.channel = channel;
  // This is super annoying 
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
  if ( expected_bytes == 0 ) {
    msg = &m_msg;
    return 1;  
  }
  printf( "expected_bytes: %02x\n", expected_bytes );
  return 0;
}
  
    
   
  

