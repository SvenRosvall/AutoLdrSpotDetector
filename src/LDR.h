#pragma once

#include "LdrState.h"

#include "Streaming.h"

template <class LDRT>
class AutoLdrSpotDetectors;

template <class LDRT, class DetectorT>
struct LDR
{
  DetectorT * parent;
  int sensorPin = -1;
  int lastValue = -1;
  LdrState state;
  int threshold = -1;

  void create(AutoLdrSpotDetectors<LDRT> * parent, int sensorPin)
  {
    this->parent = (DetectorT *) parent;
    this->sensorPin = sensorPin;
    this->state = OPEN;
  }

  virtual void setup();
  virtual void readValue();

  int value() const
  {
    return lastValue;
  }

  virtual void updateState() = 0;

  bool isCovered() const;

  virtual Print & printTitle(Print & p) const;
  virtual Print & printValue(Print & p) const;
  virtual Print & printTitleDetailed(Print & p) const;
  virtual Print & printValueDetailed(Print & p) const;
  virtual Print & printValue() const;
};

template <class LDRT>
Print & operator<<(Print & p, LDRT const & ldr);

#include "LDR.cpp"
