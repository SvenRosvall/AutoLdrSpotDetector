#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "InstantStateDecider.h"

namespace
{
  struct MockLdr : LdrBase
  {
    int lastValue;
    int threshold;

    virtual int value() const override { return lastValue; }
    virtual int getThreshold() const override { return threshold; }
  };

  void testInstantStateDecider_belowThreshold()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    InstantStateDecider decider;

    ldr.threshold = 500;
    ldr.lastValue = 499;
    assertEquals(OPEN, decider.decide(ldr));

    ldr.lastValue = 500;
    assertEquals(COVERED, decider.decide(ldr));
  }

  void testInstantStateDecider_aboveThreshold()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    InstantStateDecider decider;

    ldr.threshold = 500;
    ldr.lastValue = 501;
    assertEquals(COVERED, decider.decide(ldr));
  }

  void testInstantStateDecider_atThreshold()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    InstantStateDecider decider;

    ldr.threshold = 500;
    ldr.lastValue = 500;
    assertEquals(COVERED, decider.decide(ldr));
  }
}

void testInstantStateDecider()
{
  testInstantStateDecider_belowThreshold();
  testInstantStateDecider_aboveThreshold();
  testInstantStateDecider_atThreshold();
}