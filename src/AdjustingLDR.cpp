#include "AdjustingLDR.h"
#include "AdjustingDetectors.h"

void AdjustingLDR::resetThreshold()
{
  switch (state)
  {
    case OPEN:
    case COVERING:
      threshold = lastValue + parent->getThresholdLevel();
      break;
    case COVERED:
    case OPENING:
      threshold = lastValue - parent->getThresholdLevel();
      break;
  }
}

void AdjustingLDR::updateThreshold()
{
  switch (state)
  {
  case OPEN:
  case COVERING:
    if (lastValue + parent->getThresholdLevel() > threshold)
    {
      ++threshold;
    }
    else
    {
      --threshold;
    }
    break;

  case COVERED:
  case OPENING:
    if (lastValue - parent->getThresholdLevel() > threshold)
    {
      ++threshold;
    }
    else
    {
      --threshold;
    }
    break;
  }
}

void AdjustingLDR::setup()
{
  LDR<AdjustingLDR, AdjustingDetectors>::setup();
  resetThreshold();
}

void AdjustingLDR::updateState()
{
  updateThreshold();

  LdrState newState = stateDecider->decide(*this);
  if (state != newState)
  {
    state = newState;
    parent->onChange(this, state);
    resetThreshold();
  }
}
