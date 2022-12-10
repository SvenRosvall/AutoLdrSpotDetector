#pragma once

#include "LdrState.h"

class LdrBase;

class StateDecider
{
public:
  virtual LdrState decide() = 0;
  virtual void setState(LdrState state) { };

  class Factory
  {
  public:
    virtual StateDecider * create(LdrBase const & ldr) const = 0;
  };
};
