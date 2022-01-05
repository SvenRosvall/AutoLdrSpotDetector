#include "MovingAverageDetectors.h"
#include "MovingAverageLDR.h"

void MovingAverageDetectors::setThresholdLevel(int l)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setThresholdLevel(l);
  }
}

void MovingAverageDetectors::setMovingAverageP(float p)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setMovingAverageP(p);
  }
}

bool MovingAverageDetectors::checkOtherLDRs(MovingAverageLDR * thisLdr, LdrState checkedState)
{
  unsigned int countInState = 0;
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].sensorPin == thisLdr->sensorPin)
      continue;
    if (ldrs[i].state == checkedState)
      ++countInState;
  }
  return countInState > ldrCount / 2;
}

void MovingAverageDetectors::update()
{
  allLdrs([](MovingAverageLDR & ldr) { ldr.readValue(); });
  checkTransitions();
  allLdrs([](MovingAverageLDR & ldr) { ldr.updateState(); });
}

TransitionState MovingAverageDetectors::areLdrsChanging(LdrState transitionState, LdrState finalState)
{
  unsigned int countTransitioning = 0;
  unsigned int countTransitioned = 0;
  unsigned int countFinalState = 0;

  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].state == transitionState)
    {
      ++countTransitioning;
      if ((transitionState == COVERING )
        ? (ldrs[i].threshold > ldrs[i].lastValue)
        : (ldrs[i].threshold < ldrs[i].lastValue))
      {
        ++countTransitioned;
      }
    }
    if (ldrs[i].state == finalState)
      ++countFinalState;
  }
  DEBUG("Found " << countTransitioning << " transitioning to " << transitionState << ", " 
    << countTransitioned << " transitioned, "
    << countFinalState << " in alt state");
  if (countTransitioning == 0)
    return NOT_TRANSITIONING; // None are transitioning
  if (countTransitioning + countFinalState != ldrCount)
    return NOT_TRANSITIONING; // There is at least one LDR that never started transitioning.
  if (countTransitioned == countTransitioning)
    return TRANSITIONED; // No LDR left transitioning
  else
    return TRANSITIONING; // At least one LDR is still transitioning.
}

//int transitionCount = 0;
void MovingAverageDetectors::checkTransitions()
{
  TransitionState transitionState = areLdrsChanging(COVERING, COVERED);
  if (transitionState == TRANSITIONING || transitionState == TRANSITIONED)
  {
    DEBUG("Transitioning COVERING");
    if (transitionState == TRANSITIONED)
    {
      DEBUG("Transition time over, change to OPEN");
      changeState(COVERING, OPEN);
    }
  }
#if 0
  else
  {
    // Do we need to check for changes to opening?
    // We assume that most LDRs will be open in most cases.
    transitionState = areLdrsChanging(COVERING, COVERED);  // TODO: Should these states be the same as above?
    if (transitionState == TRANSITIONING || transitionState == TRANSITIONED)
    {
      DEBUG("Transitioning OPENING");
      if (transitionState == TRANSITIONED)
      {
        DEBUG("Transition time over, change to COVERED");
        changeState(OPENING, COVERED);
      }
    }
#endif
    else
    {
      //      transitionCount = 0;
    }
//  }
}
