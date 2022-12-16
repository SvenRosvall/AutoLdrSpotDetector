#include "TimedStateDecider.h"
#include "LdrBase.h"

TimedStateDecider::TimedStateDecider(Factory const & factory, LdrBase const & ldr)
  : StateDecider(ldr)
  , factory(factory)
{
  state = ldr.getState();
}

LdrState TimedStateDecider::decide()
{
  switch (state)
  {
  case OPEN:
    if (ldr.value() > ldr.getThreshold())
    {
      state = COVERING;
      timer = millis() + factory.getChangeCoverInterval();
    }
    return OPEN;

  case COVERING:
    if (ldr.value() < ldr.getThreshold())
    {
      state = OPEN;
      return OPEN;
    }
    else if (timer < millis())
    {
      // Time is up, change state.
      state = COVERED;
      return COVERED;
    }
    else
    {
      // Timer ticking for COVERING state.
      return OPEN;
    }

  case COVERED:
    if (ldr.value() < ldr.getThreshold())
    {
      state = OPENING;
      timer = millis() + factory.getChangeOpenInterval();
    }
    return COVERED;

  case OPENING:
    if (ldr.value() > ldr.getThreshold())
    {
      // Not OPENING anymore, revert state.
      state = COVERED;
      return COVERED;
    }
    else if (timer < millis())
    {
      // Time is up, change state.
      state = OPEN;
      return OPEN;
    }
    else
    {
      // Timer ticking for OPENING state.
      return COVERED;
    }

  default:
    return OPEN;
  }
}

TimedStateDecider::Factory const & createTimedStateDeciderFactory()
{
  static TimedStateDecider::Factory factory;
  return factory;
}
