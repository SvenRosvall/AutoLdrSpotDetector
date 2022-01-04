#include "ThresholdLDR.h"


void ThresholdLDR::updateState()
{
  LdrState newState = (lastValue < threshold) ? OPEN : COVERED;
  if (state != newState)
  {
    state = newState;
    parent->onChange(this, state);
  }
}
