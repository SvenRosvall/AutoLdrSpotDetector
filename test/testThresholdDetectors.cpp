#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "ThresholdDetectors.h"
#include "testThresholdDetectors.h"
#include <vector>
#include <iostream>

namespace
{
  const int THRESHOLD = 130;

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

  void testThresholdDetectors_construct()
  {
    test();
    clearArduinoValues();

    MockAction action;
    ThresholdDetectors detectors1(action, {A0}, THRESHOLD);
    assertEquals(-1, detectors1.getLdrs()[0].value());

    ThresholdDetectors detectors6(action, {A0, A1, A2, A3, A4, A5}, THRESHOLD);
    assertEquals(-1, detectors6.getLdrs()[0].value());
    assertEquals(-1, detectors6.getLdrs()[1].value());
    assertEquals(-1, detectors6.getLdrs()[2].value());
    assertEquals(-1, detectors6.getLdrs()[3].value());
    assertEquals(-1, detectors6.getLdrs()[4].value());
    assertEquals(-1, detectors6.getLdrs()[5].value());
  }

  void testThresholdDetectors_setup()
  {
    test();
    clearArduinoValues();

    MockAction action;
    ThresholdDetectors detectors(action, {A0, A1}, THRESHOLD);

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);

    detectors.setup();

    ThresholdLDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());
  }

  void testThresholdDetectors_update()
  {
    test();
    clearArduinoValues();

    MockAction action;
    ThresholdDetectors detectors(action, {A0, A1}, THRESHOLD);

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);
    detectors.setup();

    ThresholdLDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());

    setAnalogRead(A0, 133);
    setAnalogRead(A1, 139);
    detectors.update();

    assertEquals(133, ldrs[0].value());
    assertEquals(139, ldrs[1].value());
  }

  void showLdr(const char * msg, ThresholdLDR const & ldr)
  {
    std::cout << msg << ": state=" << ldr.state << " value=" << ldr.value() <<" threshold=" << ldr.threshold << std::endl;
  }
  void testThresholdDetectors_changeToCovered()
  {
    test();
    clearArduinoValues();

    MockAction action;
    ThresholdDetectors detectors(action, {A0, A1}, THRESHOLD);
    ThresholdLDR const * ldrs = detectors.getLdrs();

    setAnalogRead(A0, 2);
    setAnalogRead(A1, 3);
    detectors.setup();
    showLdr("after setup()   ", ldrs[0]);

    assertEquals(0, action.changes.size());
    assertEquals(OPEN, ldrs[0].state);
    assertEquals(2, ldrs[0].value());
    assertEquals(3, ldrs[1].value());

    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update()  ", ldrs[0]);

    assertEquals(1, action.changes.size());
    assertEquals(COVERED, ldrs[0].state);
    assertEquals(202, ldrs[0].value());
    assertEquals(4, ldrs[1].value());
  }
}


void testThresholdDetectors()
{
  testThresholdDetectors_construct();
  testThresholdDetectors_setup();
  testThresholdDetectors_update();
  testThresholdDetectors_changeToCovered();
}
