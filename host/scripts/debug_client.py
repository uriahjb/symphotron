'''
Code that receives data from a client device over usb
and prints it out over console

On OSX
Usage:
  $ python debug_client -i /dev/tty.usbmodemXXXX
    ( on my machine its like usbmodem1411 )

I think its basically the same on windows, just open your python interpeter
  >> run debug_client -i /dev/tty.usbmodemXXXX

'''

import sys, getopt
from time import time as now 
import serial

try:
  opts, args = getopt.getopt(sys.argv[1:],"h:i:",["iface="])
except getopt.GetoptError:
  print 'debug_client.py -i </dev/tty...>'
  sys.exit(2)

# Default filename
iface = None
for opt, arg in opts:
      if opt == '-h':
        print 'debug_client.py -i </dev/tty...>'
        sys.exit()
      elif opt in ("-i", "--iface"):
         iface = arg

if iface is None:
  print 'debug_client.py -i </dev/tty...> '
  sys.exit()

# Set up serial
#ser = serial.Serial('/dev/tty.usbmodem1421')
ser = serial.Serial(iface)

while True:
  data = ser.readline()
  if data == '':
    continue
  print repr(data)




