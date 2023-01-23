// Choose what algorithm to use.
//#define THRESHOLD_DETECTORS
//#define ADJUSTING_DETECTORS
//#define MOVING_AVERAGE_DETECTORS
//#define GROUP_MOVING_AVERAGE_DETECTORS
#define GROUP_MOVING_AVERAGE_DETECTORS_INTEGRATING

// Choose what set of output is wanted.
#define PLOT_ALL_VALUES
//#define PLOT_DETAILS 0
//#define PRINT_DEBUG

// Tuning parameters
const int INTERVAL = 50; // ms - Must be more than 10 ms to avoid interference with mains frequency.
#ifdef THRESHOLD_DETECTORS
const int THRESHOLD = 850; // good level for testing indoors without sun shining through window.
#endif
#ifdef MOVING_AVERAGE_DETECTORS
const float P = 0.050f;  // for moving average
// 0.02 is too slow. Reaction time is 2-3 seconds.
const int THRESHOLD_LEVEL = 150;
// 50 is too low. Will trigger on a train on the adjacent track.
#endif
#if defined(GROUP_MOVING_AVERAGE_DETECTORS) || defined(GROUP_MOVING_AVERAGE_DETECTORS_INTEGRATING)
const float P = 0.20f;  // for moving average
const float Q = 0.1f;  // for moving diff average
const float SelfDiffRatio = 0.2f; // How much to weigh in own LDR vs all LDRs
const int THRESHOLD_LEVEL = 150;
const int CHANGE_INTERVAL = 500; // ms
#endif

// Note that the difference between open and covered is different at bright levels (low values) and dark levels (high values).
// Sample levels for LDR 2:
// Morning window: open: 100-120 covered: 300-350
// Curtains: open: 350 covered: 750

#include <Streaming.h>
#ifdef THRESHOLD_DETECTORS
#include <ThresholdDetectors.h>
#include <InstantStateDecider.h>
#endif
#ifdef ADJUSTING_DETECTORS
#include <AdjustingDetectors.h>
#endif
#ifdef MOVING_AVERAGE_DETECTORS
#include <MovingAverageDetectors.h>
#endif
#if defined(GROUP_MOVING_AVERAGE_DETECTORS) || defined(GROUP_MOVING_AVERAGE_DETECTORS_INTEGRATING)
#include <GroupMovingAverageDetectors.h>
#endif
#if defined(GROUP_MOVING_AVERAGE_DETECTORS_INTEGRATING)
#include <IntegrationStateDecider.h>
#include <BucketPinReader.h>
#endif

#include <initializer_list.h>
#include "LedChanger.h"

// Declare pairs of LDR pin and LED pin.
std::initializer_list<LdrLedPair> ldrLedPairs = {{A0, 10}, {A1, 9}, {A2, 8}, {A3, 7}, {A4, 6}, {A5, 5}};
LedChanger ledChanger(ldrLedPairs);

#ifdef THRESHOLD_DETECTORS
InstantStateDecider::Factory stateDeciderFactory;
ThresholdDetectors detectors(ledChanger, getLdrPins(ldrLedPairs), stateDeciderFactory, THRESHOLD);
#endif
#ifdef ADJUSTING_DETECTORS
AdjustingDetectors detectors(ledChanger, getLdrPins(ldrLedPairs), 250);
#endif
#ifdef MOVING_AVERAGE_DETECTORS
MovingAverageDetectors detectors(ledChanger, getLdrPins(ldrLedPairs));
#endif
#ifdef GROUP_MOVING_AVERAGE_DETECTORS
TimedStateDecider::Factory stateDeciderFactory;
GroupMovingAverageDetectors detectors(ledChanger, getLdrPins(ldrLedPairs), stateDeciderFactory);
#endif
#ifdef GROUP_MOVING_AVERAGE_DETECTORS_INTEGRATING
IntegrationStateDecider::Factory stateDeciderFactory;
GroupMovingAverageDetectors detectors(ledChanger, getLdrPins(ldrLedPairs), stateDeciderFactory, createBucketPinReader());
#endif


void setup() {
  digitalWrite(13, HIGH);
  Serial.begin(115200);

#ifdef MOVING_AVERAGE_DETECTORS
  detectors.setMovingAverageP(P);
  detectors.setThresholdLevel(THRESHOLD_LEVEL);
#endif
#ifdef GROUP_MOVING_AVERAGE_DETECTORS
  detectors.setMovingAverageP(P);
  detectors.setMovingDiffAverageP(Q);
  detectors.setSelfDiffRatio(SelfDiffRatio);
  detectors.setThresholdLevel(THRESHOLD_LEVEL);

  stateDeciderFactory.setChangeCoverInterval(CHANGE_INTERVAL);
  stateDeciderFactory.setChangeOpenInterval(CHANGE_INTERVAL);
#endif
#ifdef GROUP_MOVING_AVERAGE_DETECTORS_INTEGRATING
  detectors.setMovingAverageP(P);
  detectors.setMovingDiffAverageP(Q);
  detectors.setSelfDiffRatio(SelfDiffRatio);
  detectors.setThresholdLevel(THRESHOLD_LEVEL);
#endif
  detectors.setup();

  // DEBUG("Start : " << ldrs);
#ifdef PLOT_DETAILS
  detectors.plotTitleDetailed(PLOT_DETAILS);
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
  detectors.plotDetailed(PLOT_DETAILS);
#endif
#ifdef PLOT_ALL_VALUES
  detectors.plotAll();
#endif
  
  lastMillis = now;
  delay(INTERVAL);  // TODO: Move to detectors.update()
}
