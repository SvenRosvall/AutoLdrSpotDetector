#pragma once

#include "LDR.h"
class GroupMovingAverageDetectors;

struct GroupMovingAverageLDR : public LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>
{
  float movingAverage = -1;
  float movingDiffAverage = 0;
  unsigned long timer;

  // Tunable parameters
  int thresholdLevel = 50;
  float movingAverageP = 0.1;
  float movingDiffAverageP = 0.3;
  float selfDiffRatio = 0.5;
  unsigned int changeInterval = 300; // ms

  void setThresholdLevel(int l)
  {
    this->thresholdLevel = l;
  }

  void setMovingAverageP(float p)
  {
    this->movingAverageP = p;
  }

  void setMovingDiffAverageP(float p)
  {
    this->movingDiffAverageP = p;
  }

  void setSelfDiffRatio(float r)
  {
    this->selfDiffRatio = r;
  }

  void setChangeInterval(unsigned int i)
  {
    this->changeInterval = i;
  }

  void setup() override;
  void readValue();
  void updateThreshold();
  void updateMovingAverage();
  virtual void updateState() override;
  virtual Print & printTitleDetailed(Print & p) const override;
  virtual Print & printValueDetailed(Print & p) const override;
};
