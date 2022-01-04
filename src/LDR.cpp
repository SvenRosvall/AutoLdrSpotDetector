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
  DEBUG("LDR pin=" << sensorPin << "(A" << sensorPin-A0 << ") value=" << lastValue);
}

template<class LDRT>
void LDR<LDRT>::readValue()
{
  lastValue = analogRead(sensorPin);
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
  p << " valA" << sensorPin-A0 << " thresholdA" << sensorPin-A0 << " stateA" << sensorPin-A0;
  return p;
}

template<class LDRT>
Print & LDR<LDRT>::printValueDetailed(Print & p) const
{
  p << " " << lastValue << " " << threshold << " " << state;
  return p;
}

template<class LDRT>
Print & LDR<LDRT>::printValue() const
{
  printValue(Serial);
  return Serial;
}

template<class LDRT>
Print & operator<<(Print & p, LDR<LDRT> const & ldr)
{
  return ldr.printValue(p);
  return Serial;
}

#endif