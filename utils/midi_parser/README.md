midi_parser
==============

A minimal midi parser for embedded platforms

Pipeline
==============

Our midi pipeline is as follows:

###### File.mid  ->  Host-Side Python-Midi Sequencer  ->  USB  ->  MCU midi_parser

Midi Parser
===========

Usage:
* Initialize midiParser passing it input queue to be filled by usb interface
* Check for input messages and handle accordingly 

##### Message Status Types:
```c
/ Channel voice messages
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
```


##### Example:
```c
extern "C" {                                                                              
#include "mGeneral.h"                                                                     
#include "mBus.h"                                                                         
#include "mUSB.h"                                                                         
}                                                                                         
                                                                                          
#include "bqueue.h"                                                                       
#include "usb_iface.h"
#include "midi_parser.h"
                                                                                          
int main( void )                                                                          
{                                                                                         
    // Initialize m4 things                                                               
    mInit();                                                                              
    //mBusInit();                                                                         
    mUSBInit();                                                                           
    
    // Initialize in/out queues
    bQueue in_q;                                                                          
    bQueue out_q;
                                                                                          
    // Set up usb interface                                                               
    usbIface usb( &in_q, &out_q );                                                        
  
    // Initialize midi parser
    midiParser midi( &in_q );
    midiMsg midi_msg;
                                                                                          
    mYellowON;                                                                            
                                                                                          
    while(1)                                                                              
    {            
    
        // Fill input queue with usb data
        usb.readBytes()
        
        // Check if message has been received
        if ( midi.hasMsg( msg ) ) {
          usb.printf("msg: \n"
                     "\tstatus: %02x\n"
                     "\tchannel: %02x\n"                                                          
                     "\tdata[0]: %02x\n"
                     "\tdata[1]: %02x\n", msg.status, msg.channel, msg.data[0], msg.data[1]);     
        }
                                                                                          
        // Write usb data out                                                                 
        usb.writeBytes();                                                                 
    }                                                                                     
}               
```

File.mid
==============

Raw midi data converted into time sorted events. 

Mary.mid
```
'MThd\x00\x00\x00\x06\x00\x01\x00\x02\x00\xdcMTrk\x00\x00\x00\x12\x00\xffX\x04\x04\x02
 \x18\x08\x00\xffY\x02\x00\x00\x01\xff/\x00MTrk\x00\x00\x00\xfb\x00\xb0[:\x00\nE\x00\x00
 \x00\x00 \x00\x00\xc0\x18\x00\x90@H\x007F\x81g@\x00\x19>H\x81g>\x00\x19<G\x81g<\x00\x19>O
 \x81N7\x00\x19>\x00\x19@U\x007O\x81g@\x00\x19@N\x81g@\x00\x19@J\x83N7\x00\x00@\x002>K
 \x007M\x81g>\x00\x19>M\x81g>\x00\x19>K\x83N7\x00\x00>\x002@R\x007O\x81g@\x00\x19CT\x81gC
 \x00\x19CK\x83N7\x00\x00C\x002@I\x007N\x81g@\x00\x19>E\x81g>\x00\x19<G\x81g<\x00\x19>P\x81N7
 \x00\x19>\x00\x19@T\x007O\x81g@\x00\x19@L\x81g@\x00\x19@J\x81g@\x00\x19@M\x81N7\x00\x19@\x00
 \x19>K\x007N\x81g>\x00\x19>J\x81g>\x00\x19@Q\x81g@\x00\x19>F\x81N7\x00\x19>\x00\x19<I\x004H
 \x87N<\x00\x004\x00\x01\xff/\x00'
```

Python-Midi Sequencer
=====================

Depends on: https://github.com/vishnubob/python-midi

Simple Python Sequencer that takes midi events and encodes them into the raw message format.
Source: http://www.midi.org/techspecs/midimessages.php

```python
'''
Python Sequencer that sends data over serial port

At the moment this assumes that the tempo is fixed
  - ie: doesn't look for tempo change events
'''

import sys
from time import time as now

from midi import read_midifile
from midi.events import *
'''
Encode midi event, shamelessly stolen and modified from
fileio.py
'''
def encode_midi_event(event):
    ret = ''
    if isinstance(event, Event):
      ret += chr(event.statusmsg | event.channel)
      ret += str.join('', map(chr, event.data))
    elif isinstance(event, MetaEvent):
      print "MetaEvent: " + str(event)
    elif isinstance(event, SysexEvent):
      print "SysexEvent: " + str(event)
    else:
        raise ValueError, "Unknown MIDI Event: " + str(event)
    return ret

# Load in midi file
filename = '../mary.mid'
pattern = read_midifile(filename)

# Convert event ticks into absolute times and sort accordingly
pattern.make_ticks_abs()
events = []
for track in pattern:
    for event in track:
        events.append(event)
# Sort events by time
events.sort()

# Convert bpm to ms_per_tick
bpm = 120
tempo = int(float(6e7) / bpm) / 1000
ms_per_tick = tempo/float(pattern.resolution)

# Write events given ms_per_tick
print "Sequencing Initialization"
t0 = now()
while len(events):
  curtime = now() - t0
  curtime
  while len(events):
    evt = events.pop(0)
    ts = (evt.tick * ms_per_tick)/1000.0
    if ts > curtime:
      events.insert(0, evt)
      break
    hxstr = ":".join("{0:x}".format(ord(c)) for c in encode_midi_event(evt))
    print evt, hxstr, curtime
print "Sequencing Complete"



```


  


