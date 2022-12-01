#pragma once

#include "LDR.h"
class MovingAverageDetectors;

class MovingAverageLDR : public LDR<MovingAverageLDR, MovingAverageDetectors>
{
  friend MovingAverageDetectors;

private:
  float movingAverage = -1;
  int oldThreshold = -1;

public:
  float getMovingAverage() const { return movingAverage; }
  float getOldThreshold() const { return oldThreshold; }

  void setup() override;
  void readValue() override;
  void updateThreshold();
  virtual void updateState() override;
  virtual Print & printTitleDetailed(Print & p) const override;
  virtual Print & printValueDetailed(Print & p) const override;
};
