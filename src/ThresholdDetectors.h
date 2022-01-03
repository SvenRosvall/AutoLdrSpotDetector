#pragma once

#include "AutoLdrSpotDetectors.h"
#include "ThresholdLDR.h"

class ThresholdDetectors : public AutoLdrSpotDetectors<ThresholdLDR>
{
  int threshold;

public:
  ThresholdDetectors(SensorChangeAction & action, const std::initializer_list<int> & il, int threshold)
    : AutoLdrSpotDetectors(action, il)
    , threshold(threshold)
  {}
};
