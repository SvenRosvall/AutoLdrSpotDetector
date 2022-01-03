#pragma once

#include "LDR.h"

class MovingAverageLDR : public LDR<MovingAverageLDR>
{
public:
  using LDR<MovingAverageLDR>::LDR;
};
