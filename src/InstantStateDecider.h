#pragma once

#include "StateDecider.h"
#include "LdrBase.h"
#include "LdrState.h"

class InstantStateDecider : public StateDecider
{
  LdrBase const & ldr;

public:
  InstantStateDecider(LdrBase const & ldr)
    : ldr(ldr)
  { }

  virtual LdrState decide() override
  {
    return (ldr.value() < ldr.getThreshold()) ? OPEN : COVERED;
  }
};