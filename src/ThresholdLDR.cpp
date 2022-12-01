#include "ThresholdLDR.h"
#include "ThresholdDetectors.h"


void ThresholdLDR::updateState()
{
  LdrState newState = stateDecider->decide(*this);
  if (state != newState)
  {
    state = newState;
    parent->onChange(this, state);
  }
}
