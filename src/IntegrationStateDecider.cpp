#include "IntegrationStateDecider.h"

#include "LdrBase.h"
#include <Streaming.h>

const int limit = 1024;

IntegrationStateDecider::IntegrationStateDecider(Factory const & factory, LdrBase const & ldr)
  : StateDecider(ldr)
  , factory(factory)
{
  state = ldr.getState();
  integratedDiff = (state == OPEN) ? -limit : limit;
}

LdrState IntegrationStateDecider::decide()
{
  int diff = ldr.value() - ldr.getThreshold();
  integratedDiff += diff;
  if (integratedDiff > limit)
  {
    integratedDiff = limit;
    return COVERED;
  }
  else if (integratedDiff < -limit)
  {
    integratedDiff = -limit;
    return OPEN;
  }
  else
  {
    return ldr.getState();
  }
}

Print & IntegrationStateDecider::printTitleDetailed(Print & p) const
{
  p << " intDiff";
  return p;
}

Print & IntegrationStateDecider::printValueDetailed(Print & p) const
{
  p << " " << integratedDiff;
  return p;
}
