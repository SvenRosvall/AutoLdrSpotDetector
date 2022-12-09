#pragma once

#include "StateDecider.h"
#include "LdrBase.h"
#include "LdrState.h"

#include <Arduino.h>

class TimedStateDecider : public StateDecider
{
public:
  class Factory;

private:
  Factory const & factory;
  LdrBase const & ldr;
  LdrState state;
  unsigned long timer = millis() + 200;

  TimedStateDecider(Factory const & factory, LdrBase const & ldr)
    : factory(factory)
    , ldr(ldr)
  {
    state = ldr.getState();
  }

public:
  virtual LdrState decide() override;

  virtual void setState(LdrState state) override
  {
    this->state = state;
  }

  class Factory
  {
    unsigned int changeCoverInterval = 500; // ms
    unsigned int changeOpenInterval = 500; // ms

  public:
    TimedStateDecider * create(LdrBase const & ldr)
    {
      return new TimedStateDecider(*this, ldr);
    }

    void setChangeCoverInterval(unsigned int i) { this->changeCoverInterval = i; }
    unsigned getChangeCoverInterval() const { return changeCoverInterval; }
    void setChangeOpenInterval(unsigned int i) { this->changeOpenInterval = i; }
    unsigned getChangeOpenInterval() const { return changeOpenInterval; }
  };
};
