#pragma once

#include "LdrState.h"

class LdrBase;

class StateDecider
{
public:
  virtual LdrState decide(LdrBase const & ldr) = 0;
  virtual void setState(LdrState state) { };
};
