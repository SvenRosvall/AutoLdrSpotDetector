#include "MovingAverageLDR.h"
#include "MovingAverageDetectors.h"

void MovingAverageLDR::setup()
{
  LDR<MovingAverageLDR, MovingAverageDetectors>::setup();
  movingAverage = lastValue;
  updateThreshold();
}

void MovingAverageLDR::readValue()
{
  LDR<MovingAverageLDR, MovingAverageDetectors>::readValue();
  float movingAverageP = parent->getMovingAverageP();
  movingAverage = movingAverageP * lastValue + (1 - movingAverageP) * movingAverage;
  updateThreshold();
}

void MovingAverageLDR::updateThreshold()
{
  switch (state)
  {
    case OPEN:
      threshold = movingAverage + parent->getThresholdLevel();
      oldThreshold = threshold;
      break;
    case COVERING:
      threshold = movingAverage + parent->getThresholdLevel();
      break;
    case COVERED:
      threshold = movingAverage - parent->getThresholdLevel();
      oldThreshold = threshold;
      break;
    case OPENING:
      threshold = movingAverage - parent->getThresholdLevel();
      break;
  }
}

void MovingAverageLDR::updateState()
{
  switch (state)
  {
    case OPEN:
      if (lastValue > threshold)
      {
        state = COVERING;
        DEBUG("LDR A" << sensorPin-A0 << " change to covering.");
      }
      break;
    case COVERING:
      if (lastValue < movingAverage)
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " change back to open.");
      }
      // TODO: Checking other LDRs shouldn't be done here. It is per detector.
      else if (movingAverage > oldThreshold && !parent->checkOtherLDRs(this, COVERING))
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " change to covered.");
        parent->onChange(this, state);
        movingAverage = lastValue;
        threshold = movingAverage - parent->getThresholdLevel();
      }
      break;
    case COVERED:
      if (lastValue < threshold)
      {
        state = OPENING;
        DEBUG("LDR A" << sensorPin-A0 << " change to opening.");
      }
      break;
    case OPENING:
      if (lastValue > movingAverage + parent->getThresholdLevel())
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " change back to covered.");
      }
      // TODO: Checking other LDRs shouldn't be done here. It is per detector.
      else if (movingAverage < oldThreshold && !parent->checkOtherLDRs(this, OPENING))
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " change to opened.");
        parent->onChange(this, state);
        movingAverage = lastValue;
        threshold = movingAverage + parent->getThresholdLevel();
      }
      break;
  }
}


Print & MovingAverageLDR::printTitleDetailed(Print & p) const
{
  LDR<MovingAverageLDR, MovingAverageDetectors>::printTitleDetailed(p);
  p << " avgA" << sensorPin-A0;
  return p;
}

Print & MovingAverageLDR::printValueDetailed(Print & p) const
{
  LDR<MovingAverageLDR, MovingAverageDetectors>::printValueDetailed(p);
  p << " " << movingAverage;
  return p;
}
