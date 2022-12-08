#pragma once

#include "LdrState.h"

class LdrBase;

class StateDecider
{
public:
  virtual LdrState decide() = 0;
  virtual void setState(LdrState state) { };
};
