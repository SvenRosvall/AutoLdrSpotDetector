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
  
  void create(AutoLdrSpotDetectors * parent, int sensorPin)
  {
    this->parent = parent;
    this->sensorPin = sensorPin;
    this->state = OPEN;
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
  Print & printValue() const;
};
