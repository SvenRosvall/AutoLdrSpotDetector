#include "Arduino.hpp"

#include "../examples/DemoWithLEDs/DemoWithLEDs.ino"

all: test

test: run-testAll srcObjects examples
	@: do nothing

run-%: % srcObjects
	./$<

CC=g++
CPPFLAGS=-std=gnu++11 -I../src -I. -g -Wall

srcObjects : AutoLdrSpotDetectors.o

testAll : testAll.o \
	testAutoLdrSpotDetectors.o \
	testArduino.o \
	AutoLdrSpotDetectors.o \
	ArduinoMock.o \
	Streaming.o \
	TestTools.o

#AutoLdrSpotDetectors.o : ../src/AutoLdrSpotDetectors.cpp
#	$(CC) $(CPPFLAGS) -c $<
AutoLdrSpotDetectors.cpp : ../src/AutoLdrSpotDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/AutoLdrSpotDetectors.cpp"' >> $@
	cat $< >> $@


examples: sketchDemoWithLeds.o

depend :
	$(CC) -MM $(CPPFLAGS) *.cpp > dependencies

include dependencies

clean :
	-rm *.o *.exe AutoLdrSpotDetectors.cpp

LIBDIR=../../libraries/AutoLdrSpotDetectors
lib : $(wildcard $(LIBDIR)/src/*.cpp) \
	$(wildcard $(LIBDIR)/src/*.h)

$(LIBDIR)/src/% : ../src/%
	cp $< $@
