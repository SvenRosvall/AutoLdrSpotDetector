#pragma once

#include "AutoLdrSpotDetectors.h"
#include "MovingAverageLDR.h"
#include "InstantStateDecider.h"

class MovingAverageDetectors : public AutoLdrSpotDetectors<MovingAverageLDR>
{
  // Tunable parameters
  int thresholdLevel = 50;
  float movingAverageP = 0.1;

public:
  MovingAverageDetectors(SensorChangeAction & action,
                         const std::initializer_list<uint8_t> & il)
    : AutoLdrSpotDetectors(action, il, createInstantStateDeciderFactory())
  {}

  void setThresholdLevel(int l) { this->thresholdLevel = l; }
  int getThresholdLevel() const { return thresholdLevel; }

  void setMovingAverageP(float p) { this->movingAverageP = p; }
  float getMovingAverageP() const { return movingAverageP; }

  virtual void update() override;

  // Called from MovingAverageLDR
  bool checkOtherLDRs(MovingAverageLDR * thisLdr, LdrState checkedState);

private:
  void checkTransitions();
  TransitionState areLdrsChanging(LdrState transitionState, LdrState finalState);
};
