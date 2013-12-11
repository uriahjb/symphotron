Midi Sequencer
===============
Requires: 
* python-midi 
  * https://github.com/vishnubob/python-midi
* pyserial
  * sudo pip install pyserial 

Installation:
* Follow installation instructions in python-midi README

Midi Video
===============
Requires: pygame

Installation (OSX):
```
$ brew install pip
$ brew install portmidi
$ pip install hg+http://bitbucket.org/pygame/pygame
```
Usage:
* Client USB Debugger:
  * $ python debug_client.py -i /dev/tty.usbmodemXXXX ( mine is 1411 )

* Midi Sequencer:
  * $ python midi_seq_raw.py -i /dev/tty.usbmodemXXXX -f your_midi_file.mid
