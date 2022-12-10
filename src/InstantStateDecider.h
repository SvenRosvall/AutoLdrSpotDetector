#pragma once

#include "StateDecider.h"
#include "LdrBase.h"
#include "LdrState.h"

class InstantStateDecider : public StateDecider
{
  LdrBase const & ldr;

  InstantStateDecider(LdrBase const & ldr)
    : ldr(ldr)
  { }

public:
  virtual LdrState decide() override
  {
    return (ldr.value() < ldr.getThreshold()) ? OPEN : COVERED;
  }

  class Factory : public StateDecider::Factory
  {
  public:
    virtual InstantStateDecider * create(LdrBase const & ldr) const override
    {
      return new InstantStateDecider(ldr);
    }
  };
};

InstantStateDecider::Factory const & createInstantStateDeciderFactory();
