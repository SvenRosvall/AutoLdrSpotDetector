#include "TimedStateDecider.h"

LdrState TimedStateDecider::decide(LdrBase const & ldr)
{
  switch (state)
  {
  case OPEN:
    if (ldr.value() > ldr.getThreshold())
    {
      state = COVERING;
      timer = millis() + 500;
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
      timer = millis() + 500;
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