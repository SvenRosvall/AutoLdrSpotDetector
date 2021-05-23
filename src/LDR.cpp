#include "LDR.h"
#include "AutoLdrSpotDetectors.h"
#include <Arduino.h>
#include <Streaming.h>

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
#define DEBUG(S) Serial << S << endl
#else
#define DEBUG(S)
#endif

void LDR::setup()
{
  pinMode(sensorPin, INPUT_PULLUP);
  lastValue = analogRead(sensorPin);
  movingAverage = lastValue;
  updateThreshold();
  DEBUG("LDR pin=" << sensorPin << "(A" << sensorPin-A0 << ") value=" << lastValue);
}

void LDR::readValue()
{
  lastValue = analogRead(sensorPin);
  movingAverage = movingAverageP * lastValue + (1 - movingAverageP) * movingAverage;
  updateThreshold();
}

void LDR::updateThreshold()
{
  switch (state)
  {
    case OPEN:
      threshold = movingAverage + thresholdLevel;
      oldThreshold = threshold;
      break;
    case COVERING:
      threshold = movingAverage + thresholdLevel;
      break;
    case COVERED:
      threshold = movingAverage - thresholdLevel;
      oldThreshold = threshold;
      break;
    case OPENING:
      threshold = movingAverage - thresholdLevel;
      break;
  }
}

void LDR::updateState()
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
      if (lastValue < movingAverage - thresholdLevel)
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " change back to open.");
      }
      // TODO: Checking other LDRs shouldn't be done here. It is per detector.
      else if (movingAverage > oldThreshold && !parent->checkOtherLDRs(this, COVERING))
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " is covered.");
        parent->onChange(this, state);
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
      if (lastValue > movingAverage + thresholdLevel)
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " change back to covered.");
      }
      // TODO: Checking other LDRs shouldn't be done here. It is per detector.
      else if (movingAverage < oldThreshold && !parent->checkOtherLDRs(this, OPENING))
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " is open.");
        parent->onChange(this, state);
      }
      break;
  }
}

Print & LDR::printTitle(Print & p) const
{
  p << " valA" << sensorPin-A0;
  return p;
}

Print & LDR::printValue(Print & p) const
{
  p << " " << lastValue;
  return p;
}

Print & LDR::printTitleDetailed(Print & p) const
{
  p << " valA" << sensorPin-A0 << " avgA" << sensorPin-A0 << " thresholdA" << sensorPin-A0 << " stateA" << sensorPin-A0;
  return p;
}

Print & LDR::printValueDetailed(Print & p) const
{
  p << " " << lastValue << " " << movingAverage << " " << threshold << " " << state;
  return p;
}

Print & LDR::printValue() const
{
  printValue(Serial);
  return Serial;
}

Print & operator<<(Print & p, LDR const & ldr)
{
  return ldr.printValue(p);
  return Serial;
}
