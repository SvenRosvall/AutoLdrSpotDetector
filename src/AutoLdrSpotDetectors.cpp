#include <Streaming.h>
#include <limits.h>

#include "AutoLdrSpotDetectors.h"

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
#define DEBUG(S) Serial << S << endl
#else
#define DEBUG(S)
#endif

// Tuning parameters
const int INTERVAL = 100; // ms
const float P = 0.10f;  // for moving average
const int THRESHOLD = 50;

void LDR::setup()
{
  pinMode(sensorPin, INPUT_PULLUP);
  lastValue = analogRead(sensorPin);
  movingAverage = lastValue;
  updateThreshold();
  DEBUG("LDR pin=" << sensorPin << "(A" << sensorPin-A0 << ") value=" << lastValue);
}

void LDR::readValue()
{
  lastValue = analogRead(sensorPin);
  movingAverage = P * lastValue + (1 - P) * movingAverage;
  updateThreshold();
}

void LDR::updateThreshold()
{
  switch (state)
  {
    case OPEN:
      threshold = movingAverage + THRESHOLD;
      oldThreshold = threshold;
      break;
    case COVERING:
      threshold = movingAverage + THRESHOLD;
      break;
    case COVERED:
      threshold = movingAverage - THRESHOLD;
      oldThreshold = threshold;
      break;
    case OPENING:
      threshold = movingAverage - THRESHOLD;
      break;
  }
}

void LDR::updateState()
{
  switch (state)
  {
    case OPEN:
      if (lastValue > threshold)
      {
        state = COVERING;
        DEBUG("LDR A" << sensorPin-A0 << " change to covering.");
      }
      break;
    case COVERING:
      if (lastValue < movingAverage - THRESHOLD)
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " change back to open.");
      }
      // TODO: Checking other LDRs shouldn't be done here. It is per detector.
      else if (movingAverage > oldThreshold && !parent->checkOtherLDRs(this, COVERING))
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " is covered.");
        parent->onChange(this, state);
      }
      break;
    case COVERED:
      if (lastValue < threshold)
      {
        state = OPENING;
        DEBUG("LDR A" << sensorPin-A0 << " change to opening.");
      }
      break;
    case OPENING:
      if (lastValue > movingAverage + THRESHOLD)
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " change back to covered.");
      }
      // TODO: Checking other LDRs shouldn't be done here. It is per detector.
      else if (movingAverage < oldThreshold && !parent->checkOtherLDRs(this, OPENING))
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " is open.");
        parent->onChange(this, state);
      }
      break;
  }
}

Print & LDR::printTitle(Print & p) const
{
#ifdef PLOT_DETAILS
  p << " valA" << sensorPin-A0 << " avgA" << sensorPin-A0 << " thresholdA" << sensorPin-A0 << " stateA" << sensorPin-A0;
#endif
#ifdef PLOT_ALL_VALUES
  p << " valA" << sensorPin-A0;
#endif
  return p;
}

Print & LDR::printValue(Print & p) const
{
#ifdef PLOT_DETAILS
  p << " " << lastValue << " " << movingAverage << " " << threshold << " " << state;
#endif
#ifdef PLOT_ALL_VALUES
  p << " " << lastValue;
#endif
  return p;
}

Print & LDR::printValue() const
{
  printValue(Serial);
  return Serial;
}

Print & operator<<(Print & p, LDR const & ldr)
{
  return ldr.printValue(p);
  return Serial;
}

bool AutoLdrSpotDetectors::checkOtherLDRs(LDR * thisLdr, LdrState checkedState)
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

void AutoLdrSpotDetectors::allLdrs(void (*f)(LDR &))
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    f(ldrs[i]);
  }
}

void AutoLdrSpotDetectors::onChange(LDR * thisLdr, LdrState newState)
{
  action.onChange(thisLdr->sensorPin, newState);
}

//Print & operator<<(Print & p, LDR ldrs[])
Print & operator<<(Print & p, AutoLdrSpotDetectors & detectors)
{
  detectors.allLdrs([](LDR & ldr){ ldr.printValue(); }); 
  return p;
}

TransitionState AutoLdrSpotDetectors::areLdrsChanging(LdrState transitionState, LdrState finalState)
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

void AutoLdrSpotDetectors::changeState(LdrState fromState, LdrState toState)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].state == fromState)
      ldrs[i].state = toState;
  }
}

int transitionCount = 0;
void AutoLdrSpotDetectors::checkTransitions()
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
      transitionCount = 0;
    }
//  }
}

AutoLdrSpotDetectors::AutoLdrSpotDetectors(SensorChangeAction & action,
                                           std::initializer_list<int> il)
  : action(action)
  , ldrCount(il.size())
{
  ldrs = new LDR[ldrCount];
  
  auto p = ldrs;
  for (auto e : il)
  {
    p++->create(this, e);
  }
}

void AutoLdrSpotDetectors::setup()
{
  allLdrs([](LDR & ldr) { ldr.setup(); });
}

void AutoLdrSpotDetectors::update()
{
  allLdrs([](LDR & ldr) { ldr.readValue(); });
  checkTransitions();
  allLdrs([](LDR & ldr) { ldr.updateState(); });
}
