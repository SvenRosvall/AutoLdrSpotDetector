#include "Arduino.hpp"

#include "../examples/DemoWithLEDs/DemoWithLEDs.ino"

all: test

test: run-testAll srcObjects examples
	@: do nothing

run-%: % srcObjects
	./$<

CC=g++
CPPFLAGS=-std=gnu++11 -I../src -I. -g -Wall

srcObjects : AutoLdrSpotDetectors.o \
    MovingAverageDetectors.o \
    LDR.o \
	Helper.o

testAll : AutoLdrSpotDetectors.o \
    MovingAverageDetectors.o \
    LDR.o \
	Helper.o \
	testAll.o \
	testMovingAverageDetectors.o \
	testArduino.o \
	ArduinoMock.o \
	Streaming.o \
	TestTools.o

AutoLdrSpotDetectors.cpp : ../src/AutoLdrSpotDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/AutoLdrSpotDetectors.cpp"' >> $@
	cat $< >> $@

MovingAverageDetectors.cpp : ../src/MovingAverageDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/MovingAverageDetectors.cpp"' >> $@
	cat $< >> $@

LDR.o : ../src/LDR.cpp
	$(CC) $(CPPFLAGS) -c $<

Helper.o : ../src/Helper.cpp
	$(CC) $(CPPFLAGS) -c $<

examples: sketchDemoWithLeds.o

depend :
	$(CC) -MM $(CPPFLAGS) *.cpp > dependencies

include dependencies

clean :
	-rm *.o *.exe AutoLdrSpotDetectors.cpp MovingAverageDetectors.cpp

LIBDIR=../../libraries/AutoLdrSpotDetectors
lib : $(wildcard $(LIBDIR)/src/*.cpp) \
	$(wildcard $(LIBDIR)/src/*.h)

$(LIBDIR)/src/% : ../src/%
	cp $< $@
