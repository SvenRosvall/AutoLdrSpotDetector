#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "GroupMovingAverageDetectors.h"
#include "GroupMovingAverageLDR.h"
#include "testGroupMovingAverageLDR.h"

// Backdoor for setting state of an LDR.
void testGroupMovingAverageLDR_setState(GroupMovingAverageLDR & ldr, LdrState state, float ma)
{
  ldr.state = state;
  ldr.movingAverage = ma;
  ldr.updateThreshold();
}

namespace
{
  class MockAction : public SensorChangeAction
  {
  public:
    void onChange(int ldrIndex, bool covered)
    {
    }
  };

  void testGroupMovingAverageLDR_construct()
  {
    GroupMovingAverageLDR ldr;
  }

  void testGroupMovingAverageLDR_updateThreshold()
  {
    MockAction action;
    TimedStateDecider::Factory stateDeciderFactory;
    GroupMovingAverageDetectors detectors(action, {A0}, stateDeciderFactory);
    GroupMovingAverageLDR & ldr = (const_cast<GroupMovingAverageLDR *>(detectors.getLdrs()))[0];
    detectors.setThresholdLevel(100);

    testGroupMovingAverageLDR_setState(ldr, OPEN, 0);
    assertEquals(100, ldr.getThreshold());

    testGroupMovingAverageLDR_setState(ldr, OPEN, 512);
    assertEquals(572, ldr.getThreshold());

    testGroupMovingAverageLDR_setState(ldr, COVERED, 512);
    assertEquals(452, ldr.getThreshold());

    testGroupMovingAverageLDR_setState(ldr, COVERED, 922);
    assertEquals(895, ldr.getThreshold());
  }
}

void testGroupMovingAverageLDR()
{
  test();
  testGroupMovingAverageLDR_construct();
  testGroupMovingAverageLDR_updateThreshold();
}
