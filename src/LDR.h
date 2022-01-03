#pragma once

#include "LdrState.h"

template <class LDRT>
class AutoLdrSpotDetectors;

template <class LDRT>
struct LDR
{
  class AutoLdrSpotDetectors<LDRT> * parent;
  int sensorPin = -1;
  int lastValue = -1;
  float movingAverage = -1;
  LdrState state;
  int threshold = -1;
  int oldThreshold = -1;

  // Tunable parameters
  int thresholdLevel = 50;
  float movingAverageP = 0.1;
  
  void create(AutoLdrSpotDetectors<LDRT> * parent, int sensorPin)
  {
    this->parent = parent;
    this->sensorPin = sensorPin;
    this->state = OPEN;
  }

  void setThresholdLevel(int l)
  {
    this->thresholdLevel = l;
  }

  void setMovingAverageP(float p)
  {
    this->movingAverageP = p;
  }

  void setup();
  void readValue();

  int value() const
  {
    return lastValue;
  }

  void updateThreshold();
  void updateState();

  bool isCovered() const;

  Print & printTitle(Print & p) const;
  Print & printValue(Print & p) const;
  Print & printTitleDetailed(Print & p) const;
  Print & printValueDetailed(Print & p) const;
  Print & printValue() const;
};

#include "LDR.cpp"
