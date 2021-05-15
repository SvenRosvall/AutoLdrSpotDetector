#include "Arduino.hpp"
#include "ArduinoMock.hpp"
#include "TestTools.hpp"

#include "AutoLdrSpotDetectors.h"
#include "testAutoLdrSpotDetectors.h"
#include <vector>
#include <iostream>

namespace
{
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

  void testAutoLdrSpotDetectors_construct()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AutoLdrSpotDetectors detectors1(action, {A0});
    assertEquals(-1, detectors1.getLdrs()[0].value());

    AutoLdrSpotDetectors detectors6(action, {A0, A1, A2, A3, A4, A5});
    assertEquals(-1, detectors6.getLdrs()[0].value());
    assertEquals(-1, detectors6.getLdrs()[1].value());
    assertEquals(-1, detectors6.getLdrs()[2].value());
    assertEquals(-1, detectors6.getLdrs()[3].value());
    assertEquals(-1, detectors6.getLdrs()[4].value());
    assertEquals(-1, detectors6.getLdrs()[5].value());
  }

  void testAutoLdrSpotDetectors_setup()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AutoLdrSpotDetectors detectors(action, {A0, A1});

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);

    detectors.setup();

    LDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());
  }

  void testAutoLdrSpotDetectors_update()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AutoLdrSpotDetectors detectors(action, {A0, A1});

    setAnalogRead(A0, 123);
    setAnalogRead(A1, 129);
    detectors.setup();

    LDR const * ldrs = detectors.getLdrs();
    assertEquals(123, ldrs[0].value());
    assertEquals(129, ldrs[1].value());

    setAnalogRead(A0, 133);
    setAnalogRead(A1, 139);
    detectors.update();

    assertEquals(133, ldrs[0].value());
    assertEquals(139, ldrs[1].value());
  }

  void showLdr(const char * msg, LDR const & ldr)
  {
    std::cout << msg << ": state=" << ldr.state << " value=" << ldr.value() << " mAvg=" << ldr.movingAverage << " threshold=" << ldr.threshold << " oldThreshold=" << ldr.oldThreshold << std::endl;
  }
  void testAutoLdrSpotDetectors_changeToCovered()
  {
    test();
    clearArduinoValues();

    MockAction action;
    AutoLdrSpotDetectors detectors(action, {A0, A1});
    LDR const * ldrs = detectors.getLdrs();

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
    showLdr("after update() 1", ldrs[0]);

    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].state);
    assertEquals(202, ldrs[0].value());
    assertEquals(4, ldrs[1].value());

    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 2", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].state);

    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 3", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].state);
    
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 4", ldrs[0]);
    assertEquals(0, action.changes.size());
    assertEquals(COVERING, ldrs[0].state);
    
    setAnalogRead(A0, 202);
    setAnalogRead(A1, 4);
    detectors.update();
    showLdr("after update() 5", ldrs[0]);
    assertEquals(1, action.changes.size());
    assertEquals(0, action.changes[0].ldrIndex);
    assertEquals(true, action.changes[0].covered);
    assertEquals(COVERED, ldrs[0].state);
  }
}


void testAutoLdrSpotDetectors()
{
  testAutoLdrSpotDetectors_construct();
  testAutoLdrSpotDetectors_setup();
  testAutoLdrSpotDetectors_update();
  testAutoLdrSpotDetectors_changeToCovered();
}
