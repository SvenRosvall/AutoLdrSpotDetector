#pragma once

#include "LDR.h"
class GroupMovingAverageDetectors;

class GroupMovingAverageLDR : public LDR<GroupMovingAverageLDR, GroupMovingAverageDetectors>
{
  friend GroupMovingAverageDetectors;
  friend void testGroupMovingAverageLDR_setState(GroupMovingAverageLDR & ldr, LdrState state, float ma);

private:
  float movingAverage = -1;
  float movingDiffAverage = 0;
  unsigned long timer;
  int avgOfOtherDiffs = 0;

private:
  void updateThreshold();
  void updateMovingAverage();
  void resetMovingAverage();
  void setAvgOfOtherDiffs(int d) { avgOfOtherDiffs = d; }

public:
  float getMovingAverage() const { return movingAverage; }

  void setup() override;
  void readValue();
  virtual void updateState() override;
  virtual Print & printTitleDetailed(Print & p) const override;
  virtual Print & printValueDetailed(Print & p) const override;
};
