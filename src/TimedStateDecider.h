#pragma once

#include "StateDecider.h"
#include "LdrBase.h"
#include "LdrState.h"

#include <Arduino.h>

class TimedStateDecider : public StateDecider
{
  LdrBase const & ldr;
  LdrState state;
  unsigned long timer = millis() + 200;
  unsigned int changeCoverInterval = 500; // ms
  unsigned int changeOpenInterval = 500; // ms

  TimedStateDecider(LdrBase const & ldr)
    : ldr(ldr)
  {
    state = ldr.getState();
  }

public:
  void setChangeInterval(unsigned int changeInterval)
  {
    setChangeInterval(changeInterval, changeInterval);
  }

  void setChangeInterval(unsigned int changeCoverInterval, unsigned int changeOpenInterval)
  {
    this->changeCoverInterval = changeCoverInterval;
    this->changeOpenInterval = changeOpenInterval;
  }

  virtual LdrState decide() override;

  virtual void setState(LdrState state) override
  {
    this->state = state;
  }

  class Factory
  {
  public:
    TimedStateDecider * create(LdrBase const & ldr)
    {
      return new TimedStateDecider(ldr);
    }
  };
};
