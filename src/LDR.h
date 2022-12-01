#pragma once

#include "LdrState.h"
#include "LdrBase.h"

#include "Streaming.h"

template <class LDRT>
class AutoLdrSpotDetectors;

template <class LDRT, class DetectorT>
class LDR : public LdrBase
{
  friend AutoLdrSpotDetectors<LDRT>;

protected:
  DetectorT * parent;
  int sensorPin = -1;
  int lastValue = -1;
  LdrState state;
  int threshold = -1;

public:
  void create(AutoLdrSpotDetectors<LDRT> * parent, int sensorPin)
  {
    this->parent = (DetectorT *) parent;
    this->sensorPin = sensorPin;
    this->state = OPEN;
  }

  virtual void setup();
  virtual void readValue();

  int value() const override { return lastValue; }
  int getState() const { return state; }
  int getThreshold() const override { return threshold; }

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
