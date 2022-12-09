#pragma once

#include "AutoLdrSpotDetectors.h"
#include "GroupMovingAverageLDR.h"
#include "TimedStateDecider.h"

class GroupMovingAverageDetectors : public AutoLdrSpotDetectors<GroupMovingAverageLDR>
{
  // Tunable parameters
  int thresholdLevel = 50;
  float movingAverageP = 0.1;
  float movingDiffAverageP = 0.3;
  float selfDiffRatio = 0.5;
  const unsigned int defaultChangeCoverInterval = 300; // ms
  const unsigned int defaultChangeOpenInterval = 300; // ms
  float thresholdScaling = 0.8;

  // Calculated values
  float avgOfDiffs;

  TimedStateDecider::Factory deciderFactory;

  float calculateAvgOfDiffs() const;

public:
  GroupMovingAverageDetectors(SensorChangeAction & action, const std::initializer_list<uint8_t> & il);

  void setThresholdLevel(int l) { this->thresholdLevel = l; }
  int getThresholdLevel() { return thresholdLevel; }

  void setMovingAverageP(float p) { this->movingAverageP = p; }
  float getMovingAverageP() { return movingAverageP; }

  void setMovingDiffAverageP(float p) { this->movingDiffAverageP = p; }
  float getMovingDiffAverageP() { return movingDiffAverageP; }

  void setSelfDiffRatio(float r) { this->selfDiffRatio = r; }
  float getSelfDiffRatio() { return selfDiffRatio; }

  void setChangeCoverInterval(unsigned int i) { deciderFactory.setChangeCoverInterval(i); }
  void setChangeOpenInterval(unsigned int i) { deciderFactory.setChangeOpenInterval(i); }

  void setThresholdScaling(float s) { this->thresholdScaling = s; }
  float getThresholdScaling() { return thresholdScaling; }

  float getAvgOfDiffs() const
  {
    return avgOfDiffs;
  }

  void setup();
  virtual void update() override;

  // Called from GroupMovingAverageLDR
  bool checkOtherLDRs(GroupMovingAverageLDR * thisLdr, LdrState checkedState);
};
