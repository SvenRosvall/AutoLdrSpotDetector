#ifndef AutoLdrSpotDetectors_cpp
#define AutoLdrSpotDetectors_cpp

#include <Streaming.h>
#include <limits.h>

// Temporarily reverse inclusion as this contains template code.
#include "AutoLdrSpotDetectors.h"

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
#define DEBUG(S) Serial << S << endl
#else
#define DEBUG(S)
#endif

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::allLdrs(void (*f)(LDRT &))
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    f(ldrs[i]);
  }
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::onChange(LDRT * thisLdr, LdrState newState)
{
  int index = thisLdr - (LDRT*) ldrs;
  action.onChange(index, newState == COVERED);
}

//Print & operator<<(Print & p, LDRT ldrs[])
template<class LDRT>
Print & operator<<(Print & p, AutoLdrSpotDetectors<LDRT> & detectors)
{
  detectors.allLdrs([](LDRT & ldr){ ldr.printValue(); }); 
  return p;
}

template<class LDRT>
TransitionState AutoLdrSpotDetectors<LDRT>::areLdrsChanging(LdrState transitionState, LdrState finalState)
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

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::changeState(LdrState fromState, LdrState toState)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].state == fromState)
      ldrs[i].state = toState;
  }
}

//int transitionCount = 0;
template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::checkTransitions()
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

template<class LDRT>
AutoLdrSpotDetectors<LDRT>::AutoLdrSpotDetectors(SensorChangeAction & action,
                                           const std::initializer_list<int> & il)
  : action(action)
  , ldrCount(il.size())
{
  ldrs = new LDRT[ldrCount];
  
  auto p = ldrs;
  for (auto e : il)
  {
    p++->create(this, e);
  }
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::setThresholdLevel(int l)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setThresholdLevel(l);
  }
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::setMovingAverageP(float p)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setMovingAverageP(p);
  }
}


template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::setup()
{
  allLdrs([](LDRT & ldr) { ldr.setup(); });
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::update()
{
  allLdrs([](LDRT & ldr) { ldr.readValue(); });
  checkTransitions();
  allLdrs([](LDRT & ldr) { ldr.updateState(); });
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::plotTitleAll()
{
  allLdrs([](LDRT & ldr) { ldr.printTitle(Serial); });
  Serial << endl;
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::plotAll()
{
  allLdrs([](LDRT & ldr) { ldr.printValue(Serial); });
  Serial << endl;
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::plotTitleDetailed(unsigned int nLdr)
{
  if (ldrCount < nLdr)
  {
    nLdr = ldrCount;
  }
  for (unsigned int i = 0 ; i < nLdr ; ++i)
  {
    ldrs[i].printTitleDetailed(Serial);
  }
  Serial << endl;
}

template<class LDRT>
void AutoLdrSpotDetectors<LDRT>::plotDetailed(unsigned int nLdr)
{
  if (ldrCount < nLdr)
  {
    nLdr = ldrCount;
  }
  for (unsigned int i = 0 ; i < nLdr ; ++i)
  {
    ldrs[i].printValueDetailed(Serial);
  }
  Serial << endl;
}

#endif
