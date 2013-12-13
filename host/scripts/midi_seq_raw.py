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

from glob import glob
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
  opts, args = getopt.getopt(sys.argv[1:],"h:i:f:a:p:n:q",["iface=","file=","addr=","port=","no_serial", "quick_start"])
except getopt.GetoptError:
  print help_str
  sys.exit(2)

# Default filename
iface = None
no_serial = False
filename = 'mary.mid'
addr = 'localhost'
port = 8181
quickstart = False
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
      elif opt in ["-n", "--no_serial"]:
         no_serial = True
      elif opt in ["-q", "--quick_start"]:
         quickstart = True

# If iface is not specified, automatically accumulate all ser divices
instruments = []
if iface is None:
  devs = glob('/dev/tty.usbmodem*')
  for dev in devs:
    ser = serial.Serial(dev)
    ser.setBaudrate(115200)
    instruments.append(ser)
  
if iface is not None:
  ser = serial.Serial(iface)
  ser.setBaudrate(115200)
  instruments.append(ser)

if len(instruments) == 0:
  if not no_serial:
    print "Error: not serial interfaces, for no-serial specify -n or --no_serial"
    sys.exit();
  print "Warning: Starting without serial interfaces"
else:
  print "Instruments found: ", instruments

# Set up serial
#ser = serial.Serial('/dev/tty.usbmodem1421')

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

if quickstart:
    # Find first note on event
    for i in xrange(0,len(events)):
        evt = events[i]
        if evt.name == 'Note On':
            break

    events = events[i:]

# Convert bpm to ms_per_tick
bpm = 120
#bpm = 240
tempo = int(float(6e7) / bpm) / 1000
ms_per_tick = tempo/float(pattern.resolution)

# Write events given ms_per_tick
# Note this probably isn't as fancy as a traditional midi sequencer
# but it is very versatile
print "Sequencing Initialization"
if quickstart:
    ts0 = (events[0].tick * ms_per_tick)/1000.0
    t0 = now() - ts0
else:
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
    encoded_evt = encode_midi_event(evt) 
    for instrument in instruments:
      instrument.write( encoded_evt )
    '''
    if iface is not None:
      ser.write( encode_midi_event(evt) )
    '''
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




