#pragma once

#include "LDR.h"
class MovingAverageDetectors;

class MovingAverageLDR : public LDR<MovingAverageLDR, MovingAverageDetectors>
{
  friend MovingAverageDetectors;

private:
  float movingAverage = -1;
  int oldThreshold = -1;

  // Tunable parameters
  int thresholdLevel = 50;
  float movingAverageP = 0.1;

public:
  float getMovingAverage() const { return movingAverage; }
  float getOldThreshold() const { return oldThreshold; }

  void setThresholdLevel(int l)
  {
    this->thresholdLevel = l;
  }

  void setMovingAverageP(float p)
  {
    this->movingAverageP = p;
  }

  void setup() override;
  void readValue() override;
  void updateThreshold();
  virtual void updateState() override;
  virtual Print & printTitleDetailed(Print & p) const override;
  virtual Print & printValueDetailed(Print & p) const override;
};
