// Choose what algorithm to use.
//#define MOVING_AVERAGE_DETECTORS
#define THRESHOLD_DETECTORS

// Choose what set of output is wanted.
#define PLOT_ALL_VALUES
//#define PLOT_DETAILS
//#define PRINT_DEBUG

// Tuning parameters
const int INTERVAL = 100; // ms
#ifdef MOVING_AVERAGE_DETECTORS
const float P = 0.050f;  // for moving average
// 0.02 is too slow. Reaction time is 2-3 seconds.
const int THRESHOLD_LEVEL = 150;
// 50 is too low. Will trigger on a train on the adjacent track.
#endif
#ifdef THRESHOLD_DETECTORS
const int THRESHOLD = 850; // good level for testing indoors without sun shining through window.
#endif

// Note that the difference between open and covered is different at bright levels (low values) and dark levels (high values).
// Sample levels for LDR 2:
// Morning window: open: 100-120 covered: 300-350
// Curtains: open: 350 covered: 750

#include <Streaming.h>
#ifdef MOVING_AVERAGE_DETECTORS
#include <MovingAverageDetectors.h>
#endif
#ifdef THRESHOLD_DETECTORS
#include <ThresholdDetectors.h>
#endif
#include <initializer_list.h>
#include "LedChanger.h"

// TODO: Need to declare pairs of LDR pin and LED pin. E.g. {{A0, 10}, {A1, 9}, {A2, 8}, {A3, 7}, {A4, 6}, {A5, 5}}
std::initializer_list<LdrLedPair> ldrLedPairs = {{A0, 10}, {A1, 9}, {A2, 8}, {A3, 7}, {A4, 6}, {A5, 5}};

//LedChanger ledChanger({10, 9, 8, 7, 6, 5});
LedChanger ledChanger(ldrLedPairs);
#ifdef MOVING_AVERAGE_DETECTORS
MovingAverageDetectors detectors(ledChanger, {A0, A1, A2, A3, A4, A5});
#endif
#ifdef THRESHOLD_DETECTORS
ThresholdDetectors detectors(ledChanger, getLdrPins(ldrLedPairs), THRESHOLD);
#endif


void setup() {
  digitalWrite(13, HIGH);
  Serial.begin(115200);
  Serial.println("Setup()");

#ifdef MOVING_AVERAGE_DETECTORS
  detectors.setMovingAverageP(P);
  detectors.setThresholdLevel(THRESHOLD_LEVEL);
#endif
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
