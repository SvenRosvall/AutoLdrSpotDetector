#ifndef LDR_cpp
#define LDR_cpp

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

template<class LDRT>
void LDR<LDRT>::setup()
{
  pinMode(sensorPin, INPUT_PULLUP);
  lastValue = analogRead(sensorPin);
  movingAverage = lastValue;
  updateThreshold();
  DEBUG("LDR pin=" << sensorPin << "(A" << sensorPin-A0 << ") value=" << lastValue);
}

template<class LDRT>
void LDR<LDRT>::readValue()
{
  lastValue = analogRead(sensorPin);
  movingAverage = movingAverageP * lastValue + (1 - movingAverageP) * movingAverage;
  updateThreshold();
}

template<class LDRT>
void LDR<LDRT>::updateThreshold()
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

template<class LDRT>
void LDR<LDRT>::updateState()
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

template<class LDRT>
bool LDR<LDRT>::isCovered() const
{
  switch (state)
  {
    case OPEN: case COVERING:
      return false;
    case COVERED: case OPENING:
      return true;
    default:
      DEBUG("Unknown LDR state: " << state);
      return false;
  }
}

template<class LDRT>
Print & LDR<LDRT>::printTitle(Print & p) const
{
  p << " valA" << sensorPin-A0;
  return p;
}

template<class LDRT>
Print & LDR<LDRT>::printValue(Print & p) const
{
  p << " " << lastValue;
  return p;
}

template<class LDRT>
Print & LDR<LDRT>::printTitleDetailed(Print & p) const
{
  p << " valA" << sensorPin-A0 << " avgA" << sensorPin-A0 << " thresholdA" << sensorPin-A0 << " stateA" << sensorPin-A0;
  return p;
}

template<class LDRT>
Print & LDR<LDRT>::printValueDetailed(Print & p) const
{
  p << " " << lastValue << " " << movingAverage << " " << threshold << " " << state;
  return p;
}

template<class LDRT>
Print & LDR<LDRT>::printValue() const
{
  printValue(Serial);
  return Serial;
}

template<class LDRT>
Print & operator<<(Print & p, LDRT const & ldr)
{
  return ldr.printValue(p);
  return Serial;
}

#endif
