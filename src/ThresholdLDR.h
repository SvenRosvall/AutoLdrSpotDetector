#pragma once

#include "LDR.h"
#include "StateDecider.h"

class ThresholdDetectors;

class ThresholdLDR : public LDR<ThresholdLDR, ThresholdDetectors>
{
  friend ThresholdDetectors;

  StateDecider * stateDecider;

public:
  using LDR<ThresholdLDR, ThresholdDetectors>::LDR;

  virtual void updateState() override;
};
