#pragma once

#include "StateDecider.h"

class LdrBase;
enum LdrState;

#include <Arduino.h>

class IntegrationStateDecider : public StateDecider
{
public:
  class Factory;

private:
  Factory const & factory;
  LdrState state;
  int integratedDiff;

  IntegrationStateDecider(Factory const & factory, LdrBase const & ldr);

public:
  virtual LdrState decide() override;

  virtual void setState(LdrState state) override
  {
    this->state = state;
  }

  virtual Print & printTitleDetailed(Print & p) const;
  virtual Print & printValueDetailed(Print & p) const;

  class Factory : public StateDecider::Factory
  {
    // TODO: Possible parameters for scaling how fast the intDiff rises/falls.

  public:
    virtual IntegrationStateDecider * create(LdrBase const & ldr) const override
    {
      return new IntegrationStateDecider(*this, ldr);
    }
  };
};

IntegrationStateDecider::Factory const & createIntegrationStateDeciderFactory();
