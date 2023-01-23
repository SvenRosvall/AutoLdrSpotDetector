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
    InstantStateDecider.o \
    TimedStateDecider.o \
    IntegrationStateDecider.o \
    SimplePinReader.o \
    BucketPinReader.o \
    Helper.o

srcObjects : $(SRC_OBJECTS)

testAll : ${SRC_OBJECTS} \
    testAll.o \
    testInstantStateDecider.o \
    testTimedStateDecider.o \
    testIntegrationStateDecider.o \
    testThresholdDetectors.o \
    testAdjustingDetectors.o \
    testMovingAverageDetectors.o \
    testGroupMovingAverageDetectors.o \
    testGroupMovingAverageLDR.o \
    testArduino.o \
    ArduinoMock.o \
    Streaming.o \
    TestTools.o

AutoLdrSpotDetectors.o : ../src/AutoLdrSpotDetectors.cpp
	$(CC) $(CPPFLAGS) -c $<

ThresholdDetectors.o : ../src/ThresholdDetectors.cpp
	$(CC) $(CPPFLAGS) -c $<

AdjustingDetectors.o : ../src/AdjustingDetectors.cpp
	$(CC) $(CPPFLAGS) -c $<

MovingAverageDetectors.o : ../src/MovingAverageDetectors.cpp
	$(CC) $(CPPFLAGS) -c $<

GroupMovingAverageDetectors.o : ../src/GroupMovingAverageDetectors.cpp
	$(CC) $(CPPFLAGS) -c $<

SimplePinReader.o : ../src/SimplePinReader.cpp
	$(CC) $(CPPFLAGS) -c $<

BucketPinReader.o : ../src/BucketPinReader.cpp
	$(CC) $(CPPFLAGS) -c $<

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

InstantStateDecider.o : ../src/InstantStateDecider.cpp
	$(CC) $(CPPFLAGS) -c $<

TimedStateDecider.o : ../src/TimedStateDecider.cpp
	$(CC) $(CPPFLAGS) -c $<

IntegrationStateDecider.o : ../src/IntegrationStateDecider.cpp
	$(CC) $(CPPFLAGS) -c $<

Helper.o : ../src/Helper.cpp
	$(CC) $(CPPFLAGS) -c $<

examples: sketchDemoWithLeds.o \
	sketchMonitorLDRs.o

depend :
	$(CC) -MM $(CPPFLAGS) *.cpp > dependencies

include dependencies

clean :
	-rm *.o *.exe

LIBDIR=../../libraries/AutoLdrSpotDetectors
lib : $(wildcard $(LIBDIR)/src/*.cpp) \
	$(wildcard $(LIBDIR)/src/*.h)

$(LIBDIR)/src/% : ../src/%
	cp $< $@
