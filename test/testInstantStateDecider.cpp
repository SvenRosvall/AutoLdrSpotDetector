#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "InstantStateDecider.h"
#include "LdrBase.h"

namespace
{
  struct MockLdr : LdrBase
  {
    int lastValue;
    int threshold;

    virtual LdrState getState() const override { return OPEN; }
    virtual int value() const override { return lastValue; }
    virtual int getThreshold() const override { return threshold; }
  };

  void testInstantStateDecider_belowThreshold()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    InstantStateDecider::Factory deciderFactory;
    InstantStateDecider decider = * deciderFactory.create(ldr);

    ldr.threshold = 500;
    ldr.lastValue = 499;
    assertEquals(OPEN, decider.decide());

    ldr.lastValue = 500;
    assertEquals(COVERED, decider.decide());
  }

  void testInstantStateDecider_aboveThreshold()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    InstantStateDecider::Factory deciderFactory;
    InstantStateDecider * decider = deciderFactory.create(ldr);

    ldr.threshold = 500;
    ldr.lastValue = 501;
    assertEquals(COVERED, decider->decide());
  }

  void testInstantStateDecider_atThreshold()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    InstantStateDecider::Factory deciderFactory;
    InstantStateDecider * decider = deciderFactory.create(ldr);

    ldr.threshold = 500;
    ldr.lastValue = 500;
    assertEquals(COVERED, decider->decide());
  }
}

void testInstantStateDecider()
{
  testInstantStateDecider_belowThreshold();
  testInstantStateDecider_aboveThreshold();
  testInstantStateDecider_atThreshold();
}