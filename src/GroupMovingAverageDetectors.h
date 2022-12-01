#pragma once

#include "AutoLdrSpotDetectors.h"
#include "GroupMovingAverageLDR.h"

class GroupMovingAverageDetectors : public AutoLdrSpotDetectors<GroupMovingAverageLDR>
{
  // Tunable parameters
  int thresholdLevel = 50;
  float movingAverageP = 0.1;
  float movingDiffAverageP = 0.3;
  float selfDiffRatio = 0.5;
  unsigned int changeCoverInterval = 300; // ms
  unsigned int changeOpenInterval = 300; // ms
  float thresholdScaling = 0.8;

  // Calculated values
  float avgOfDiffs;

  float calculateAvgOfDiffs() const;

public:
  using AutoLdrSpotDetectors::AutoLdrSpotDetectors;

  void setThresholdLevel(int l) { this->thresholdLevel = l; }
  int getThresholdLevel() { return thresholdLevel; }

  void setMovingAverageP(float p) { this->movingAverageP = p; }
  float getMovingAverageP() { return movingAverageP; }

  void setMovingDiffAverageP(float p) { this->movingDiffAverageP = p; }
  float getMovingDiffAverageP() { return movingDiffAverageP; }

  void setSelfDiffRatio(float r) { this->selfDiffRatio = r; }
  float getSelfDiffRatio() { return selfDiffRatio; }

  void setChangeCoverInterval(unsigned int i) { this->changeCoverInterval = i; }
  unsigned int getChangeCoverInterval() { return changeCoverInterval; }

  void setChangeOpenInterval(unsigned int i) { this->changeOpenInterval = i; }
  unsigned int getChangeOpenInterval() { return changeOpenInterval; }

  void setThresholdScaling(float s) { this->thresholdScaling = s; }
  float getThresholdScaling() { return thresholdScaling; }

  float getAvgOfDiffs() const
  {
    return avgOfDiffs;
  }

  virtual void update() override;

  // Called from GroupMovingAverageLDR
  bool checkOtherLDRs(GroupMovingAverageLDR * thisLdr, LdrState checkedState);
};
