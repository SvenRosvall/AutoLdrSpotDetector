#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "GroupMovingAverageDetectors.h"
#include "GroupMovingAverageLDR.h"
#include "testGroupMovingAverageLDR.h"

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
    GroupMovingAverageDetectors detectors(action, {A0});
    GroupMovingAverageLDR & ldr = (const_cast<GroupMovingAverageLDR *>(detectors.getLdrs()))[0];
    detectors.setThresholdLevel(100);

    ldr.movingAverage = 0;
    ldr.updateThreshold();
    assertEquals(100, ldr.threshold);

    ldr.movingAverage = 512;
    ldr.updateThreshold();
    assertEquals(572, ldr.threshold);

    ldr.state = COVERED;

    ldr.movingAverage = 512;
    ldr.updateThreshold();
    assertEquals(452, ldr.threshold);

    ldr.movingAverage = 922;
    ldr.updateThreshold();
    assertEquals(895, ldr.threshold);
  }
}

void testGroupMovingAverageLDR()
{
  test();
  testGroupMovingAverageLDR_construct();
  testGroupMovingAverageLDR_updateThreshold();
}
