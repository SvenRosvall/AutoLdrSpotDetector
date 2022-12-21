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
  // TODO: Scaling code from GroupMovingAverageLDR
  // TODO: Make this configurable here too.
  //const float S = parent->getThresholdScaling(); // How much to take scaling into account. 1.0 => totally. 0.0 => Use threshold level as is.
  const float S = 0.8;
  float scale = (1-S) + S * (1024 - ldr.value()) / 1024.0f;

  int diff = ldr.value() - ldr.getThreshold();
  diff = diff / scale; // Compensate for low diffs at dark levels.
  integratedDiff += diff;
  if (integratedDiff >= limit)
  {
    integratedDiff = limit;
    return COVERED;
  }
  else if (integratedDiff <= -limit)
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
