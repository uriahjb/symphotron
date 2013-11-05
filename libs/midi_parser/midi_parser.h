/*
  Lightweight Midi Parser for embedded systems
*/

/*
  Status byte definitions from:
    http://www.midi.org/techspecs/midimessages.php
*/

// Channel voice messages
#define NOTE_OFF 0x80
#define NOTE_ON  0x90
#define POLYPHONIC_AFTERTOUCH 0xA0
#define MODE_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0
#define CHANNEL_AFTERTOUCH 0xD0
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




