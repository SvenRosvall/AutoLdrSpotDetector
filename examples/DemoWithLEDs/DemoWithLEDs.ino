
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
#include <initializer_list.h>

struct LdrLedPair
{
  int ldrPin;
  int ledPin;
};

std::initializer_list<int> getLdrPins(std::initializer_list<LdrLedPair> il)
{
  int * ret = new int[il.size()];
  auto p = ret;
  for (auto e : il)
  {
    *p++ = e.ldrPin;
  }
  return std::initializer_list<int>(ret, il.size());
}

class LedChanger : public SensorChangeAction
{
  int * leds;
public:
  LedChanger(std::initializer_list<int> il);
  LedChanger(std::initializer_list<LdrLedPair> il);
  void onChange(int ldrIndex, bool covered);
};

LedChanger::LedChanger(std::initializer_list<int> il)
{
  leds = new int[il.size()];
  auto p = leds;
  for (auto e : il)
  {
    *p++ = e;
    pinMode(e, OUTPUT);
    Serial << "LedChanger with LED pin=" << e << endl;
  }
}

LedChanger::LedChanger(std::initializer_list<LdrLedPair> il)
{
  leds = new int[il.size()];
  auto p = leds;
  for (auto e : il)
  {
    *p++ = e.ledPin;
    pinMode(e.ledPin, OUTPUT);
    Serial << "LedChanger with LED pin=" << e.ledPin << endl;
  }
}

void LedChanger::onChange(int ldrIndex, bool covered)
{
  digitalWrite(leds[ldrIndex], covered ? HIGH : LOW);
  Serial << "LedChanger changing led=" << ldrIndex << " pin=" << leds[ldrIndex] << " to " << (covered ? "HIGH" : "LOW") << endl;
}

// TODO: Need to declare pairs of LDR pin and LED pin. E.g. {{A0, 10}, {A1, 9}, {A2, 8}, {A3, 7}, {A4, 6}, {A5, 5}}
std::initializer_list<LdrLedPair> ldrLedPairs = {{A0, 10}, {A1, 9}, {A2, 8}, {A3, 7}, {A4, 6}, {A5, 5}};

//LedChanger ledChanger({10, 9, 8, 7, 6, 5});
LedChanger ledChanger(ldrLedPairs);
//AutoLdrSpotDetectors detectors(ledChanger, {A0, A1, A2, A3, A4, A5});
AutoLdrSpotDetectors detectors(ledChanger, getLdrPins(ldrLedPairs));


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
