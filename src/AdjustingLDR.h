#pragma once

#include "LDR.h"
class AdjustingDetectors;

class AdjustingLDR : public LDR<AdjustingLDR, AdjustingDetectors>
{
  unsigned long timer;

public:

  void setup() override;
  virtual void updateState() override;
};
