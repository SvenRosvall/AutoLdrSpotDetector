#pragma once

#include "StateDecider.h"
#include "LdrBase.h"
#include "LdrState.h"

class TimedStateDecider : public StateDecider
{
  LdrBase const & ldr;
  LdrState state;
  unsigned long timer = millis() + 200;

public:
  TimedStateDecider(LdrBase const & ldr)
    : ldr(ldr)
  {
    state = ldr.getState();
  }

  virtual LdrState decide(LdrBase const & ldr) override;
};
