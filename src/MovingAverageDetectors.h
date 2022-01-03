#pragma once

#include "AutoLdrSpotDetectors.h"
#include "MovingAverageLDR.h"

class MovingAverageDetectors : public AutoLdrSpotDetectors<MovingAverageLDR>
{
public:
  using AutoLdrSpotDetectors::AutoLdrSpotDetectors;
};
