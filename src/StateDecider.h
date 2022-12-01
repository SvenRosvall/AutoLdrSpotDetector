#pragma once

#include "LdrState.h"

class LdrBase;

class StateDecider
{
public:
  virtual LdrState decide(LdrBase const & ldr) const = 0;
};
