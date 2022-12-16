#pragma once

#include "StateDecider.h"

class LdrBase;
enum LdrState;

class InstantStateDecider : public StateDecider
{
  using StateDecider::StateDecider;

public:
  virtual LdrState decide() override;

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
