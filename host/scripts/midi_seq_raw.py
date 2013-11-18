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

# Convert event ticks into absolute times
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
# Note this probably isn't as fancy as a traditional midi sequencer
# but it is very versatile
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




