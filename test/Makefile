all: test

test: run-testAll srcObjects examples
	@: do nothing

run-%: % srcObjects
	./$<

CC=$(CXX)  # Used for linking.
CXXFLAGS=-std=gnu++11 -I../src -I. -g -Wall

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

%.o : ../src/%.cpp
	$(CXX) $(CXXFLAGS) -c $<

examples: sketchDemoWithLeds.o \
	sketchMonitorLDRs.o

depend :
	$(CXX) -MM $(CXXFLAGS) *.cpp > dependencies

include dependencies

clean :
	-rm *.o *.exe

LIBDIR=../../libraries/AutoLdrSpotDetectors
lib : $(wildcard $(LIBDIR)/src/*.cpp) \
	$(wildcard $(LIBDIR)/src/*.h)

$(LIBDIR)/src/% : ../src/%
	cp $< $@
