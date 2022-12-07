// Implement Julian Coles' self-adjusting LDRs.
// This implementation is provided to compare performance between this and the other implementations.

#pragma once

#include "AutoLdrSpotDetectors.h"
#include "AdjustingLDR.h"

class AdjustingDetectors : public AutoLdrSpotDetectors<AdjustingLDR>
{
  int thresholdLevel;

public:
  AdjustingDetectors(SensorChangeAction & action, const std::initializer_list<uint8_t> & il, int thresholdLevel = 100);

  int getThresholdLevel() const { return thresholdLevel; }

  void setup();
  virtual void update() override;
};
