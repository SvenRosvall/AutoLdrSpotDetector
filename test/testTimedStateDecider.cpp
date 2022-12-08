#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "TimedStateDecider.h"
#include "LdrBase.h"

namespace
{
  struct MockLdr : LdrBase
  {
    int lastValue;
    int threshold;
    LdrState state;

    virtual LdrState getState() const override { return state; }
    virtual int value() const override { return lastValue; }
    virtual int getThreshold() const override { return threshold; }
  };

  void testTimedStateDecider_belowThreshold_noChange()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 499;
    ldr.state = OPEN;

    TimedStateDecider decider(ldr);

    assertEquals(OPEN, decider.decide());

    addMillis(300);
    assertEquals(OPEN, decider.decide());

    addMillis(300);
    assertEquals(OPEN, decider.decide());
  }

  void testTimedStateDecider_aboveThreshold_noChange()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 501;
    ldr.state = COVERED;

    TimedStateDecider decider(ldr);

    assertEquals(COVERED, decider.decide());

    addMillis(300);
    assertEquals(COVERED, decider.decide());

    addMillis(300);
    assertEquals(COVERED, decider.decide());
  }

  void testTimedStateDecider_changeCovered()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 499;
    ldr.state = OPEN;

    TimedStateDecider decider(ldr);

    assertEquals(OPEN, decider.decide());

    addMillis(300);
    ldr.lastValue = 501;
    assertEquals(OPEN, decider.decide());

    addMillis(300);
    assertEquals(OPEN, decider.decide());

    addMillis(300);
    assertEquals(COVERED, decider.decide());
  }

  void testTimedStateDecider_changeOpen()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 501;
    ldr.state = COVERED;

    TimedStateDecider decider(ldr);

    assertEquals(COVERED, decider.decide());

    addMillis(300);
    ldr.lastValue = 499;
    assertEquals(COVERED, decider.decide());

    addMillis(300);
    assertEquals(COVERED, decider.decide());

    addMillis(300);
    assertEquals(OPEN, decider.decide());
  }

  void testTimedStateDecider_changeCovered_after1s()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 499;
    ldr.state = OPEN;

    TimedStateDecider decider(ldr);
    decider.setChangeInterval(1000);

    assertEquals(OPEN, decider.decide());

    addMillis(300);
    ldr.lastValue = 501;
    assertEquals(OPEN, decider.decide());

    addMillis(800);
    assertEquals(OPEN, decider.decide());

    addMillis(300);
    assertEquals(COVERED, decider.decide());
  }

  void testTimedStateDecider_changeOpen_after1s()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 501;
    ldr.state = COVERED;

    TimedStateDecider decider(ldr);
    decider.setChangeInterval(1000);

    assertEquals(COVERED, decider.decide());

    addMillis(300);
    ldr.lastValue = 499;
    assertEquals(COVERED, decider.decide());

    addMillis(800);
    assertEquals(COVERED, decider.decide());

    addMillis(300);
    assertEquals(OPEN, decider.decide());
  }
}

void testTimedStateDecider()
{
  testTimedStateDecider_belowThreshold_noChange();
  testTimedStateDecider_aboveThreshold_noChange();
  testTimedStateDecider_changeCovered();
  testTimedStateDecider_changeOpen();
  testTimedStateDecider_changeCovered_after1s();
  testTimedStateDecider_changeOpen_after1s();
}