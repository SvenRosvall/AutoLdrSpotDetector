#pragma once

#include "AutoLdrSpotDetectors.h"
#include "ThresholdLDR.h"

class ThresholdDetectors : public AutoLdrSpotDetectors<ThresholdLDR>
{
  StateDecider::Factory const & deciderFactory;

public:
  ThresholdDetectors(SensorChangeAction & action, const std::initializer_list<uint8_t> & il, StateDecider::Factory const & deciderFactory, int threshold);
  ThresholdDetectors(SensorChangeAction & action, const std::initializer_list<uint8_t> & il, int threshold);

  void setup();
  virtual void update() override;
};
