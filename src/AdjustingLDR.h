#pragma once

#include "LDR.h"

class AdjustingDetectors;

class AdjustingLDR : public LDR<AdjustingLDR, AdjustingDetectors>
{
  friend AdjustingDetectors;

  void resetThreshold();
  void updateThreshold();

public:

  void setup() override;
  virtual void updateState() override;
};
