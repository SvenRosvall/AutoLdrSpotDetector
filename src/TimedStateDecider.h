#pragma once

#include "StateDecider.h"
#include "LdrBase.h"
#include "LdrState.h"

class TimedStateDecider : public StateDecider
{
  LdrBase const & ldr;
  LdrState state;
  unsigned long timer = millis() + 200;
  unsigned int changeCoverInterval = 500; // ms
  unsigned int changeOpenInterval = 500; // ms

public:
  TimedStateDecider(LdrBase const & ldr)
    : ldr(ldr)
  {
    state = ldr.getState();
  }

  void setChangeInterval(unsigned int changeInterval)
  {
    setChangeInterval(changeInterval, changeInterval);
  }

  void setChangeInterval(unsigned int changeCoverInterval, unsigned int changeOpenInterval)
  {
    this->changeCoverInterval = changeCoverInterval;
    this->changeOpenInterval = changeOpenInterval;
  }

  virtual LdrState decide(LdrBase const & ldr) override;
};
