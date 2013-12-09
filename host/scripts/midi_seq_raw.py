'''
Python Sequencer that sends data over serial port and udp

At the moment this assumes that the tempo is fixed
  - ie: doesn't look for tempo change events
'''

import sys, getopt
from time import time as now 
import serial

from socket import socket, AF_INET, SOCK_DGRAM
from json import dumps

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

def jsonify_midi_event(event):
    if isinstance(event, Event):
      return {'statusmsg':event.statusmsg
             ,'channel':event.channel
             ,'data':event.data}
    elif isinstance(event, MetaEvent):
      print "MetaEvent: " + str(event)
    elif isinstance(event, SysexEvent):
      print "SysexEvent: " + str(event)
    else:
        raise ValueError, "Unknown MIDI Event: " + str(event)
    

help_str = 'midi_raw_seq.py -i </dev/tty...> -f <midi_file>'

try:
  opts, args = getopt.getopt(sys.argv[1:],"h:i:f:a:p:",["iface=","file=","addr=","port="])
except getopt.GetoptError:
  print help_str
  sys.exit(2)

# Default filename
iface = None
filename = 'mary.mid'
addr = 'localhost'
port = 8181
for opt, arg in opts:
      if opt == '-h':
        print help_str 
        sys.exit()
      elif opt in ("-i", "--iface"):
         iface = arg
      elif opt in ("-f", "--file"):
         filename = arg
      elif opt in ("-a", "--addr"):
         addr = arg
      elif opt in ("-p", "--port"):
         port = int(arg)

if iface is None:
  print "Starting iface without serial interface"

# Set up serial
#ser = serial.Serial('/dev/tty.usbmodem1421')
if iface is not None:
  ser = serial.Serial(iface)

# Initialize udp socket
sock = socket( SOCK_DGRAM, AF_INET )
sock.connect( (addr,port) )

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
#bpm = 120
bpm = 240
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
    # Write serial out
    if iface is not None:
      ser.write( encode_midi_event(evt) )
    # Try to write socket out
    try:
      msg = jsonify_midi_event(evt)
      if msg:
        sock.send( dumps(msg) )
    except KeyboardInterrupt:
      break
    except:
      pass
    print evt, hxstr, curtime
print "Sequencing Complete"

'''
while True:
  data = ser.readline()
  if data == '':
    break
  print repr(data)
'''




