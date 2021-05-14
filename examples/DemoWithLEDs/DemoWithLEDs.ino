
// Choose what set of output is wanted.
//#define PLOT_ALL_VALUES
//#define PLOT_DETAILS
//#define PRINT_DEBUG

// Tuning parameters
const int INTERVAL = 100; // ms
const float P = 0.10f;  // for moving average
const int THRESHOLD = 50;

#include <Streaming.h>
#include <AutoLdrSpotDetectors.h>
#include <stdarg.h>

class LedChanger : public SensorChangeAction
{
  int * leds;
public:
  LedChanger(int ledCount, ...);
  void onChange(int ldrIndex, bool covered);
};

LedChanger::LedChanger(int ledCount, ...)
{
  leds = new int[ledCount];
  va_list args;
  va_start(args, ledCount);
  for (int i = 0 ; i < ledCount ; ++i)
  {
    leds[i] = va_arg(args, int);
    pinMode(leds[i], OUTPUT);
    Serial << "LedChanger with LED pin=" << leds[i] << endl;
  }
  va_end(args);
}

void LedChanger::onChange(int ldrIndex, bool covered)
{
  digitalWrite(leds[ldrIndex], covered ? HIGH : LOW);
  Serial << "LedChanger changing led=" << ldrIndex << " pin=" << leds[ldrIndex] << " to " << (covered ? "HIGH" : "LOW") << endl;
}

LedChanger ledChanger(6, 10, 9, 8, 7, 6, 5);
AutoLdrSpotDetectors detectors(ledChanger, 6, A0, A1, A2, A3, A4, A5);


void setup() {
  digitalWrite(13, HIGH);
  Serial.begin(115200);
  Serial.println("Setup()");

  detectors.setup();

  // DEBUG("Start : " << ldrs);
#ifdef PLOT_DETAILS
  detectors.plotTitleDetailed(2);
#endif
#ifdef PLOT_ALL_VALUES
  detectors.plotTitleAll();
#endif
}

long lastMillis = millis();
void loop() {
  //long startMillis = millis();

  detectors.update();

  long now = millis();
//  Serial << _WIDTH(now-lastMillis,3)
//         << _WIDTH(now-startMillis,3)
//         << endl;

#ifdef PLOT_DETAILS
  detectors.plotDetailed(2);
#endif
#ifdef PLOT_ALL_VALUES
  detectors.plotAll();
#endif
  
  lastMillis = now;
  delay(INTERVAL);  // TODO: Move to detectors.update()
}
