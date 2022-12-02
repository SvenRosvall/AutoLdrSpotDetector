#pragma once

#include "LDR.h"
#include "StateDecider.h"

class AdjustingDetectors;

class AdjustingLDR : public LDR<AdjustingLDR, AdjustingDetectors>
{
  friend AdjustingDetectors;

  StateDecider * stateDecider;

  void resetThreshold();
  void updateThreshold();

public:

  void setup() override;
  virtual void updateState() override;
};
