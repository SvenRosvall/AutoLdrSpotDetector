#pragma once

#include "LdrState.h"

#include "Streaming.h"

template <class LDRT>
class AutoLdrSpotDetectors;

template <class LDRT>
struct LDR
{
  class AutoLdrSpotDetectors<LDRT> * parent;
  int sensorPin = -1;
  int lastValue = -1;
  LdrState state;
  int threshold = -1;

  void create(AutoLdrSpotDetectors<LDRT> * parent, int sensorPin)
  {
    this->parent = parent;
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
Print & operator<<(Print & p, LDR<LDRT> const & ldr);

#include "LDR.cpp"
