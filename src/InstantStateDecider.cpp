#include "InstantStateDecider.h"
#include "LdrBase.h"

LdrState InstantStateDecider::decide()
{
  return (ldr.value() < ldr.getThreshold()) ? OPEN : COVERED;
}

InstantStateDecider::Factory const & createInstantStateDeciderFactory()
{
  static InstantStateDecider::Factory factory;
  return factory;
}
