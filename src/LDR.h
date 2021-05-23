#pragma once

#include "LdrState.h"

struct LDR
{
  class AutoLdrSpotDetectors * parent;
  int sensorPin = -1;
  int lastValue = -1;
  float movingAverage = -1;
  LdrState state;
  int threshold = -1;
  int oldThreshold = -1;

  // Tunable parameters
  int thresholdLevel = 50;
  float movingAverageP = 0.1;
  
  void create(AutoLdrSpotDetectors * parent, int sensorPin)
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

  Print & printTitle(Print & p) const;
  Print & printValue(Print & p) const;
  Print & printTitleDetailed(Print & p) const;
  Print & printValueDetailed(Print & p) const;
  Print & printValue() const;
};
