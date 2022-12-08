#include "ThresholdLDR.h"
#include "ThresholdDetectors.h"


void ThresholdLDR::updateState()
{
  LdrState newState = stateDecider->decide();
  if (state != newState)
  {
    state = newState;
    parent->onChange(this, state);
  }
}
