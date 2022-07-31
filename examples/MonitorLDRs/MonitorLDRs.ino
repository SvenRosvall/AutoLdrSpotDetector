// This sketch monitors each LDR value and plots it to the Serial Plotter in the Arduino IDE.
// It doesn't do anything for threshold changes so use the simplest algorithm.


#include <AdjustingDetectors.h>

class DummyEmitter : public SensorChangeAction
{
public:
  void onChange(int, bool) override
  {}
};
DummyEmitter dummyEmitter;

const int INTERVAL = 100; // milliseconds.
auto LDR_PINS = {A0, A1, A2, A3, A4, A5, A6, A7};

AdjustingDetectors detectors(dummyEmitter, LDR_PINS, 250);


void setup()
{
  Serial.begin (115200);

  detectors.setup();
  detectors.plotTitleAll();
}

unsigned long lastRunTime = millis();
void runDetectorsTimely()
{
  unsigned long now = millis();
  if (now - lastRunTime < INTERVAL)
  {
    return;
  }
  lastRunTime = now;

  detectors.update();
  detectors.plotAll();
}

void loop()
{
  runDetectorsTimely();
}
