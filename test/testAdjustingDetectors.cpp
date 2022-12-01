#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "AdjustingDetectors.h"
#include "AdjustingLDR.h"
#include "testAdjustingDetectors.h"
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

  void testAdjustingDetectors_construct()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AdjustingDetectors detectors1(action, {A0}, THRESHOLD);
    assertEquals(-1, detectors1.getLdrs()[0].value());

    AdjustingDetectors detectors6(action, {A0, A1, A2, A3, A4, A5}, THRESHOLD);
    assertEquals(-1, detectors6.getLdrs()[0].value());
    assertEquals(-1, detectors6.getLdrs()[1].value());
    assertEquals(-1, detectors6.getLdrs()[2].value());
    assertEquals(-1, detectors6.getLdrs()[3].value());
    assertEquals(-1, detectors6.getLdrs()[4].value());
    assertEquals(-1, detectors6.getLdrs()[5].value());
  }

  void testAdjustingDetectors_setup()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AdjustingDetectors detectors(action, {A0, A1}, THRESHOLD);

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);

    detectors.setup();

    AdjustingLDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());
  }

  void testAdjustingDetectors_update()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AdjustingDetectors detectors(action, {A0, A1}, THRESHOLD);

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);
    detectors.setup();

    AdjustingLDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());

    setAnalogRead(A0, 133);
    setAnalogRead(A1, 139);
    detectors.update();

    assertEquals(133, ldrs[0].value());
    assertEquals(139, ldrs[1].value());
  }

  void showLdr(const char * msg, AdjustingLDR const & ldr)
  {
    std::cout << msg << ": state=" << ldr.getState() << " value=" << ldr.value() <<" threshold=" << ldr.getThreshold() << std::endl;
  }
  void testAdjustingDetectors_changeToCovered()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AdjustingDetectors detectors(action, {A0, A1}, THRESHOLD);
    AdjustingLDR const * ldrs = detectors.getLdrs();

    setAnalogRead(A0, 2);
    setAnalogRead(A1, 3);
    detectors.setup();
    showLdr("after setup()   ", ldrs[0]);

    assertEquals(0, action.changes.size());
    assertEquals(OPEN, ldrs[0].getState());
    assertEquals(2, ldrs[0].value());
    assertEquals(3, ldrs[1].value());

    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 1 ", ldrs[0]);

    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].getState());
    assertEquals(202, ldrs[0].value());
    assertEquals(4, ldrs[1].value());

    addMillis(101);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 2", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].getState());

    addMillis(100);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 3", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].getState());
    
    addMillis(100);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 4", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].getState());
    
    addMillis(100);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 5", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].getState());
    
    addMillis(100);
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 6", ldrs[0]);
    assertEquals(1, action.changes.size());
    assertEquals(0, action.changes[0].ldrIndex);
    assertEquals(true, action.changes[0].covered);
    assertEquals(COVERED, ldrs[0].getState());
  }
}


void testAdjustingDetectors()
{
  testAdjustingDetectors_construct();
  testAdjustingDetectors_setup();
  testAdjustingDetectors_update();
  testAdjustingDetectors_changeToCovered();
}
