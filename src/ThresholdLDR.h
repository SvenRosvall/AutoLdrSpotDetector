#pragma once

#include "LDR.h"
class ThresholdDetectors;

class ThresholdLDR : public LDR<ThresholdLDR, ThresholdDetectors>
{
public:
  using LDR<ThresholdLDR, ThresholdDetectors>::LDR;

  virtual void updateState() override;
};
