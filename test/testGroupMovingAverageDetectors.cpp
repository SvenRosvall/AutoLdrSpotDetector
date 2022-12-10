#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "GroupMovingAverageDetectors.h"
#include "GroupMovingAverageLDR.h"
#include "testGroupMovingAverageDetectors.h"
#include <vector>
#include <iostream>

namespace
{
  auto LED_PINS_A = {A0};
  // "auto" type doesn't work with numbers as their type is "int".
  // Must explicitly say that the type is "uint8_t".
  std::initializer_list<uint8_t> LED_PINS_D = {20};
  
  class MockAction : public SensorChangeAction
  {
  public:
    struct ChangeEntry
    {
      int ldrIndex;
      bool covered;
    };
    std::vector<ChangeEntry> changes;

    void onChange(int ldrIndex, bool covered)
    {
      changes.push_back({ldrIndex, covered});
    }
  };

  void testGroupMovingAverageDetectors_construct()
  {
    test();
    clearArduinoValues();

    MockAction action;
    TimedStateDecider::Factory stateDeciderFactory;
    GroupMovingAverageDetectors detectors1(action, {A0}, stateDeciderFactory);
    assertEquals(-1, detectors1.getLdrs()[0].value());

    GroupMovingAverageDetectors detectors2(action, LED_PINS_A, stateDeciderFactory);
    GroupMovingAverageDetectors detectors3(action, LED_PINS_D);

    GroupMovingAverageDetectors detectors6(action, {A0, A1, A2, A3, A4, A5}, stateDeciderFactory);
    assertEquals(-1, detectors6.getLdrs()[0].value());
    assertEquals(-1, detectors6.getLdrs()[1].value());
    assertEquals(-1, detectors6.getLdrs()[2].value());
    assertEquals(-1, detectors6.getLdrs()[3].value());
    assertEquals(-1, detectors6.getLdrs()[4].value());
    assertEquals(-1, detectors6.getLdrs()[5].value());
  }

  void testGroupMovingAverageDetectors_setup()
  {
    test();
    clearArduinoValues();

    MockAction action;
    TimedStateDecider::Factory stateDeciderFactory;
    GroupMovingAverageDetectors detectors(action, {A0, A1}, stateDeciderFactory);

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);

    detectors.setup();

    GroupMovingAverageLDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());
  }

  void testGroupMovingAverageDetectors_update()
  {
    test();
    clearArduinoValues();

    MockAction action;
    TimedStateDecider::Factory stateDeciderFactory;
    GroupMovingAverageDetectors detectors(action, {A0, A1}, stateDeciderFactory);

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);
    detectors.setup();

    GroupMovingAverageLDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());

    setAnalogRead(A0, 133);
    setAnalogRead(A1, 139);
    detectors.update();

    assertEquals(133, ldrs[0].value());
    assertEquals(139, ldrs[1].value());
  }

  void showLdr(const char * msg, GroupMovingAverageLDR const & ldr)
  {
    std::cout << msg << ": state=" << ldr.getState() << " value=" << ldr.value() << " mAvg=" << ldr.getMovingAverage() << " threshold=" << ldr.getThreshold() << std::endl;
  }

  void testGroupMovingAverageDetectors_changeToCovered()
  {
    test();
    clearArduinoValues();

    MockAction action;
    TimedStateDecider::Factory stateDeciderFactory;
    stateDeciderFactory.setChangeCoverInterval(20);
    stateDeciderFactory.setChangeOpenInterval(20);
    GroupMovingAverageDetectors detectors(action, {A0, A1}, stateDeciderFactory);
    GroupMovingAverageLDR const * ldrs = detectors.getLdrs();

    setAnalogRead(A0, 2);
    setAnalogRead(A1, 4);
    detectors.setup();
    showLdr("after setup()   ", ldrs[0]);

    assertEquals(0, action.changes.size());
    assertEquals(OPEN, ldrs[0].getState());
    assertEquals(2, ldrs[0].value());
    assertEquals(4, ldrs[1].value());

    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 1", ldrs[0]);

    assertEquals(0, action.changes.size());
    assertEquals(OPEN, ldrs[0].getState());
    assertEquals(202, ldrs[0].value());
    assertEquals(4, ldrs[1].value());

    addMillis(10);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 2", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(OPEN, ldrs[0].getState());

    addMillis(10);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 3", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(OPEN, ldrs[0].getState());

    addMillis(10);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 4", ldrs[0]);
    assertEquals(1, action.changes.size());
    assertEquals(0, action.changes[0].ldrIndex);
    assertEquals(true, action.changes[0].covered);
    assertEquals(COVERED, ldrs[0].getState());
  }
}


void testGroupMovingAverageDetectors()
{
  testGroupMovingAverageDetectors_construct();
  testGroupMovingAverageDetectors_setup();
  testGroupMovingAverageDetectors_update();
  testGroupMovingAverageDetectors_changeToCovered();
}
