all: test

test: run-testAll srcObjects examples
	@: do nothing

run-%: % srcObjects
	./$<

CC=g++
CPPFLAGS=-std=gnu++11 -I../src -I. -g -Wall

SRC_OBJECTS=AutoLdrSpotDetectors.o \
    ThresholdDetectors.o \
    AdjustingDetectors.o \
    MovingAverageDetectors.o \
    GroupMovingAverageDetectors.o \
    LDR.o \
    ThresholdLDR.o \
    AdjustingLDR.o \
    MovingAverageLDR.o \
    GroupMovingAverageLDR.o \
    Helper.o

srcObjects : $(SRC_OBJECTS)

testAll : ${SRC_OBJECTS} \
    testAll.o \
    testInstantStateDecider.o \
    testThresholdDetectors.o \
    testAdjustingDetectors.o \
    testMovingAverageDetectors.o \
    testGroupMovingAverageDetectors.o \
    testGroupMovingAverageLDR.o \
    testArduino.o \
    ArduinoMock.o \
    Streaming.o \
    TestTools.o

AutoLdrSpotDetectors.cpp : ../src/AutoLdrSpotDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/AutoLdrSpotDetectors.cpp"' >> $@
	cat $< >> $@

ThresholdDetectors.cpp : ../src/ThresholdDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/ThresholdDetectors.cpp"' >> $@
	cat $< >> $@

AdjustingDetectors.cpp : ../src/AdjustingDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/AdjustingDetectors.cpp"' >> $@
	cat $< >> $@

MovingAverageDetectors.cpp : ../src/MovingAverageDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/MovingAverageDetectors.cpp"' >> $@
	cat $< >> $@

GroupMovingAverageDetectors.cpp : ../src/GroupMovingAverageDetectors.cpp
	echo '#include "Arduino.hpp"' > $@
	echo '#line 1 "../src/GroupMovingAverageDetectors.cpp"' >> $@
	cat $< >> $@

LDR.o : ../src/LDR.cpp
	$(CC) $(CPPFLAGS) -c $<

ThresholdLDR.o : ../src/ThresholdLDR.cpp
	$(CC) $(CPPFLAGS) -c $<

AdjustingLDR.o : ../src/AdjustingLDR.cpp
	$(CC) $(CPPFLAGS) -c $<

MovingAverageLDR.o : ../src/MovingAverageLDR.cpp
	$(CC) $(CPPFLAGS) -c $<

GroupMovingAverageLDR.o : ../src/GroupMovingAverageLDR.cpp
	$(CC) $(CPPFLAGS) -c $<

Helper.o : ../src/Helper.cpp
	$(CC) $(CPPFLAGS) -c $<

examples: sketchDemoWithLeds.o \
	sketchMonitorLDRs.o

depend :
	$(CC) -MM $(CPPFLAGS) *.cpp > dependencies

include dependencies

clean :
	-rm *.o *.exe AutoLdrSpotDetectors.cpp \
		ThresholdDetectors.cpp AdjustingDetectors.cpp \
		MovingAverageDetectors.cpp GroupMovingAverageDetectors.cpp

LIBDIR=../../libraries/AutoLdrSpotDetectors
lib : $(wildcard $(LIBDIR)/src/*.cpp) \
	$(wildcard $(LIBDIR)/src/*.h)

$(LIBDIR)/src/% : ../src/%
	cp $< $@
