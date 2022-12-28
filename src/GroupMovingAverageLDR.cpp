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
#if 1
  // Use the difference value of all LDRs to determine new moving average.
  // The other LDRs are used here to react quicker for changes in ambient light.

  // add up all diffs for all LDRs
  float allLdrDiffAverage = parent->getAvgOfDiffs();

  // - Apply a portion of the sum of diffs and a portion of diff for this LDR.
  float R = parent->getSelfDiffRatio();
  float P = parent->getMovingAverageP();
  float diffToApply = R * (lastValue - movingAverage) + (1 - R) * allLdrDiffAverage;
  movingAverage += P * diffToApply;
#else
  // Use exponential smoothing for the average.
  float P = parent->getMovingAverageP();
  movingAverage = P * lastValue + (1 - P) * movingAverage;
#endif
}

void GroupMovingAverageLDR::resetMovingAverage()
{
  movingAverage = lastValue;
}

void GroupMovingAverageLDR::updateThreshold()
{
  // Threshold shall be set a bit below or above the average level.
  // How much below/above depends on the current ambient light level.
  // For bright conditions (low levels) the threshold must be set wider
  // than for dark conditions (high levels).
  // Use the configured threshold level and scale it depending on the light level.
  const float S = parent->getThresholdScaling(); // How much to take scaling into account. 1.0 => totally. 0.0 => Use threshold level as is.
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

  LdrState newState = stateDecider->decide();
  if (state != newState)
  {
    if (parent->checkOtherLDRs(this, (newState == COVERED) ? COVERING : OPENING))
    {
      // Other LDR's are also changing.
      // I.e. ambient light is changing so stay where we are.
      stateDecider->setState(state);
    }
    else
    {
      // Not enough change in ambient light.
      // Do the state transition.
      state = newState;
      parent->onChange(this, state);
      resetMovingAverage();
      updateThreshold();
    }
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
