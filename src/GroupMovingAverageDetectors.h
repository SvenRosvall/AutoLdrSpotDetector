#pragma once

#include "AutoLdrSpotDetectors.h"
#include "GroupMovingAverageLDR.h"

class GroupMovingAverageDetectors : public AutoLdrSpotDetectors<GroupMovingAverageLDR>
{
  float avgOfDiffs;

  float calculateAvgOfDiffs() const;
public:
  using AutoLdrSpotDetectors::AutoLdrSpotDetectors;
  void setThresholdLevel(int l);
  void setMovingAverageP(float p);
  void setMovingDiffAverageP(float p);
  void setSelfDiffRatio(float r);
  void setChangeInterval(unsigned int interval);
  float getAvgOfDiffs() const
  {
    return avgOfDiffs;
  }

  virtual void update() override;
};
