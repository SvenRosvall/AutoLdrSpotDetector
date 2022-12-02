#pragma once

#include "StateDecider.h"
#include "LdrBase.h"
#include "LdrState.h"

class InstantStateDecider : public StateDecider
{
public:
  virtual LdrState decide(LdrBase const & ldr) override
  {
    return (ldr.value() < ldr.getThreshold()) ? OPEN : COVERED;
  }
};