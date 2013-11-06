
CC = g++
CFLAGS = -g -Os 
INCLUDES = -I../
SRC = ../*.cpp

all: bqueue midi_parser

bqueue: ; $(CC) test_bqueue.cpp $(CFLAGS) $(INCLUDES) $(SRC) -o build/test_bqueue

midi_parser: ; $(CC) test_parser.cpp $(CFLAGS) $(INCLUDES) $(SRC) -o build/test_parser

clean: ; rm -rf build/*


