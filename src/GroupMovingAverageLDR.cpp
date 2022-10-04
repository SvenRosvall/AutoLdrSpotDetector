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
  threshold = movingAverage + parent->getThresholdLevel();
}

void GroupMovingAverageLDR::readValue()
{
  LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>::readValue();
  float P = parent->getMovingDiffAverageP();
  movingDiffAverage = P * (lastValue - movingAverage) + (1 - P) * movingDiffAverage;
}

void GroupMovingAverageLDR::updateMovingAverage()
{
  // add up all movingDiffAverage for all LDRs
  float allLdrMovingDiffAverage = parent->getAvgOfDiffs();

#if 0
  // This didn't work well. It can make the threshold overshoot the value
  // and thus cause a false state change.
  // TODO: Review this idea if it can be tuned better or shall be scrapped.

  // - Apply a portion of the sum of diffs and a portion of diff for this LDR.
  float R = parent->getSelfDiffRatio();
  float diffToApply = R * movingDiffAverage + (1 - R) * allLdrMovingDiffAverage;
  movingAverage += parent->getMovingAverageP() * diffToApply;
#else
  // Use exponential smooting for the average.
  float P = parent->getMovingAverageP();
  movingAverage = P * lastValue + (1 - P) * movingAverage;
#endif
}

void GroupMovingAverageLDR::updateThreshold()
{
  // Threshold shall be set a bit below or above the average level.
  // How much below/above depends on the current ambient light level.
  // For bright conditions (low levels) the threshold must be set wider
  // than for dark conditions (high levels).
  // Use the configured threshold level and scale it depending on the light level.
  const float S = 0.7; // How much to take scaling into account. 1.0 => totally. 0.0 => Use threshold level as is.
  float scale = (1-S) + S * (1024 - movingAverage) / 1024.0f;
  int adjustedThresholdLevel = scale * parent->getThresholdLevel() ;
  switch (state)
  {
    case OPEN:
    case COVERING:
      threshold = movingAverage + adjustedThresholdLevel;
      break;
    case COVERED:
    case OPENING:
      threshold = movingAverage - adjustedThresholdLevel;
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
        timer = millis() + parent->getChangeCoverInterval();
        DEBUG("LDR A" << sensorPin-A0 << " change to covering.");
        DEBUG("changeCoverInterval = " << parent->getChangeCoverInterval());
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
        updateThreshold();
      }
      break;
    case COVERED:
      if (lastValue < threshold)
      {
        state = OPENING;
        timer = millis() + parent->getChangeOpenInterval();
        DEBUG("LDR A" << sensorPin-A0 << " change to opening.");
        DEBUG("changeOpenInterval = " << parent->getChangeOpenInterval());
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
        updateThreshold();
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
