#include "GroupMovingAverageLDR.h"
#include "GroupMovingAverageDetectors.h"

#include <Streaming.h>

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
#define DEBUG(S) Serial << S << endl
#else
#define DEBUG(S)
#endif

void GroupMovingAverageLDR::setup()
{
  LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>::setup();
  movingAverage = lastValue;
  state = OPEN;
  threshold = movingAverage + thresholdLevel;
}

void GroupMovingAverageLDR::readValue()
{
  LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>::readValue();
  movingDiffAverage = movingDiffAverageP * (lastValue - movingAverage) + (1 - movingDiffAverageP) * movingDiffAverage;
}

void GroupMovingAverageLDR::updateMovingAverage()
{
  // add up all movingDiffAverage for all LDRs
  float allLdrMovingDiffAverage = parent->getAvgOfDiffs();

  // - Apply a portion of the sum of diffs and a portion of diff for this LDR.
  float diffToApply = selfDiffRatio * movingDiffAverage + (1-selfDiffRatio) * allLdrMovingDiffAverage;
  movingAverage += movingAverageP * diffToApply;
}

void GroupMovingAverageLDR::updateThreshold()
{
  switch (state)
  {
    case OPEN:
    case COVERING:
      threshold = movingAverage + thresholdLevel;
      break;
    case COVERED:
    case OPENING:
      threshold = movingAverage - thresholdLevel;
      break;
  }
}

void GroupMovingAverageLDR::updateState()
{
  updateMovingAverage();
  updateThreshold();

  switch (state)
  {
    case OPEN:
      if (lastValue > threshold)
      {
        state = COVERING;
        timer = millis() + changeInterval;
        DEBUG("LDR A" << sensorPin-A0 << " change to covering.");
        DEBUG("changeInterval = " << changeInterval);
      }
      break;
    case COVERING:
      if (lastValue < threshold)
      {
        // Not above threshold anymore. Revert to OPEN.
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " change back to open.");
      }
      else if (millis() > timer && !parent->checkOtherLDRs(this, COVERING))
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " change to covered.");
        parent->onChange(this, state);
        movingAverage = lastValue;
        threshold = movingAverage - thresholdLevel;
      }
      break;
    case COVERED:
      if (lastValue < threshold)
      {
        state = OPENING;
        timer = millis() + changeInterval;
        DEBUG("LDR A" << sensorPin-A0 << " change to opening.");
        DEBUG("changeInterval = " << changeInterval);
      }
      break;
    case OPENING:
      if (lastValue > threshold)
      {
        // Not above threshold anymore. Revert to COVERED.
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " change back to covered.");
      }
      else if (millis() > timer && !parent->checkOtherLDRs(this, OPENING))
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " change to opened.");
        parent->onChange(this, state);
        movingAverage = lastValue;
        threshold = movingAverage + thresholdLevel;
      }
      break;
   }
}


Print & GroupMovingAverageLDR::printTitleDetailed(Print & p) const
{
  LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>::printTitleDetailed(p);
  p << " avgA" << sensorPin-A0 << " avgDiffA" << sensorPin-A0;
  return p;
}

Print & GroupMovingAverageLDR::printValueDetailed(Print & p) const
{
  LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>::printValueDetailed(p);
  p << " " << movingAverage << " " << movingDiffAverage;
  return p;
}
