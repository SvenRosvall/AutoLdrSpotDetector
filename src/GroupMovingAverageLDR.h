#pragma once

#include "LDR.h"
class GroupMovingAverageDetectors;

struct GroupMovingAverageLDR : public LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>
{
  float movingAverage = -1;
  float movingDiffAverage = 0;
  unsigned long timer;

  void setup() override;
  void readValue();
  void updateThreshold();
  void updateMovingAverage();
  virtual void updateState() override;
  virtual Print & printTitleDetailed(Print & p) const override;
  virtual Print & printValueDetailed(Print & p) const override;
};
