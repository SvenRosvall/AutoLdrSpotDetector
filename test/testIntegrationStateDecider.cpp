#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "IntegrationStateDecider.h"
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

  void testIntegrationStateDecider_belowThreshold_noChange()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 300;
    ldr.state = OPEN;

    IntegrationStateDecider::Factory deciderFactory;
    IntegrationStateDecider * decider = deciderFactory.create(ldr);

    assertEquals(OPEN, decider->decide());
    assertEquals(OPEN, decider->decide());
    assertEquals(OPEN, decider->decide());
  }

  void testIntegrationStateDecider_aboveThreshold_noChange()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 700;
    ldr.state = COVERED;

    IntegrationStateDecider::Factory deciderFactory;
    IntegrationStateDecider * decider = deciderFactory.create(ldr);

    assertEquals(COVERED, decider->decide());
    assertEquals(COVERED, decider->decide());
    assertEquals(COVERED, decider->decide());
  }

  void testIntegrationStateDecider_changeCovered()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 499;
    ldr.state = OPEN;

    IntegrationStateDecider::Factory deciderFactory;
    IntegrationStateDecider * decider = deciderFactory.create(ldr);

    assertEquals(OPEN, decider->decide());

    ldr.lastValue = 800;
    assertEquals(OPEN, decider->decide());
    assertEquals(OPEN, decider->decide());
    assertEquals(OPEN, decider->decide());
    assertEquals(OPEN, decider->decide());
    assertEquals(OPEN, decider->decide());
    assertEquals(OPEN, decider->decide());
    assertEquals(COVERED, decider->decide());
  }

  void testIntegrationStateDecider_changeOpen()
  {
    test();
    clearArduinoValues();

    MockLdr ldr;
    ldr.threshold = 500;
    ldr.lastValue = 501;
    ldr.state = COVERED;

    IntegrationStateDecider::Factory deciderFactory;
    IntegrationStateDecider * decider = deciderFactory.create(ldr);

    assertEquals(COVERED, decider->decide());

    ldr.lastValue = 200;
    assertEquals(COVERED, decider->decide());
    assertEquals(COVERED, decider->decide());
    assertEquals(COVERED, decider->decide());
    assertEquals(COVERED, decider->decide());
    assertEquals(COVERED, decider->decide());
    assertEquals(COVERED, decider->decide());
    assertEquals(OPEN, decider->decide());
  }
}

void testIntegrationStateDecider()
{
  testIntegrationStateDecider_belowThreshold_noChange();
  testIntegrationStateDecider_aboveThreshold_noChange();
  testIntegrationStateDecider_changeCovered();
  testIntegrationStateDecider_changeOpen();
}
