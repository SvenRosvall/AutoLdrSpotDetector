#pragma once

class LdrBase;

class StateDecider
{
public:
  virtual LdrState decide(LdrBase const & ldr) const = 0;
};
