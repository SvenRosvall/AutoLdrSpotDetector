#pragma once

#include "AutoLdrSpotDetectors.h"
#include "MovingAverageLDR.h"

class MovingAverageDetectors : public AutoLdrSpotDetectors<MovingAverageLDR>
{
public:
  using AutoLdrSpotDetectors::AutoLdrSpotDetectors;
  void setThresholdLevel(int l);
  void setMovingAverageP(float p);

  virtual void update() override;

  // Called from MovingAverageLDR
  bool checkOtherLDRs(MovingAverageLDR * thisLdr, LdrState checkedState);

private:
  void checkTransitions();
  TransitionState areLdrsChanging(LdrState transitionState, LdrState finalState);
};
