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

template<class LDRT, class DetectorT>
void LDR<LDRT, DetectorT>::setup()
{
  pinMode(sensorPin, INPUT_PULLUP);
  lastValue = analogRead(sensorPin);
  DEBUG("LDR pin=" << sensorPin << "(A" << sensorPin-A0 << ") value=" << lastValue);
}

template<class LDRT, class DetectorT>
void LDR<LDRT, DetectorT>::readValue()
{
  lastValue = analogRead(sensorPin);
}

template<class LDRT, class DetectorT>
bool LDR<LDRT, DetectorT>::isCovered() const
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

template<class LDRT, class DetectorT>
Print & LDR<LDRT, DetectorT>::printTitle(Print & p) const
{
  p << " valA" << sensorPin-A0;
  return p;
}

template<class LDRT, class DetectorT>
Print & LDR<LDRT, DetectorT>::printValue(Print & p) const
{
  p << " " << lastValue;
  return p;
}

template<class LDRT, class DetectorT>
Print & LDR<LDRT, DetectorT>::printTitleDetailed(Print & p) const
{
  p << " valA" << sensorPin-A0 << " thresholdA" << sensorPin-A0 << " stateA" << sensorPin-A0;
  stateDecider->printTitleDetailed(p);
  return p;
}

template<class LDRT, class DetectorT>
Print & LDR<LDRT, DetectorT>::printValueDetailed(Print & p) const
{
  p << " " << lastValue << " " << threshold << " " << state;
  stateDecider->printValueDetailed(p);
  return p;
}

template<class LDRT, class DetectorT>
Print & LDR<LDRT, DetectorT>::printValue() const
{
  printValue(Serial);
  return Serial;
}

template<class LDRT, class DetectorT>
Print & operator<<(Print & p, LDR<LDRT, DetectorT> const & ldr)
{
  ldr.printValue(p);
  return Serial;
}

#endif
