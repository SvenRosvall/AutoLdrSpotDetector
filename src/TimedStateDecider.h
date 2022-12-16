#pragma once

#include "StateDecider.h"

class LdrBase;
enum LdrState;

#include <Arduino.h>

class TimedStateDecider : public StateDecider
{
public:
  class Factory;

private:
  Factory const & factory;
  LdrState state;
  unsigned long timer = millis() + 200;

  TimedStateDecider(Factory const & factory, LdrBase const & ldr);

public:
  virtual LdrState decide() override;

  virtual void setState(LdrState state) override
  {
    this->state = state;
  }

  class Factory : public StateDecider::Factory
  {
    unsigned int changeCoverInterval = 500; // ms
    unsigned int changeOpenInterval = 500; // ms

  public:
    virtual TimedStateDecider * create(LdrBase const & ldr) const override
    {
      return new TimedStateDecider(*this, ldr);
    }

    void setChangeCoverInterval(unsigned int i) { this->changeCoverInterval = i; }
    unsigned getChangeCoverInterval() const { return changeCoverInterval; }
    void setChangeOpenInterval(unsigned int i) { this->changeOpenInterval = i; }
    unsigned getChangeOpenInterval() const { return changeOpenInterval; }
  };
};

TimedStateDecider::Factory const & createTimedStateDeciderFactory();
