#pragma once

#include <initializer_list.h>
#include <Arduino.h>

#include "LdrState.h"

template <class LDRT, class DetectorT>
class LDR;

class SensorChangeAction
{
public:
  virtual void onChange(int ldrIndex, bool covered) = 0;
};

enum TransitionState
{
  NOT_TRANSITIONING, TRANSITIONING, TRANSITIONED
};

template <class LDRT>
class AutoLdrSpotDetectors
{
protected:
  SensorChangeAction & action;
  unsigned int ldrCount;
  LDRT * ldrs;

  void changeState(LdrState fromState, LdrState toState);

public:
  AutoLdrSpotDetectors(SensorChangeAction & action, const std::initializer_list<uint8_t> & il);

  void setup();
  virtual void update() = 0;

  void allLdrs(void (*f)(LDRT &));
  void onChange(LDRT * thisLdr, LdrState newState);

  void plotTitleAll();
  void plotAll();
  void plotTitleDetailed(unsigned int nLdr);
  void plotDetailed(unsigned int nLdr);

  LDRT const * getLdrs() const
  {
    return ldrs;
  }

  int getLdrCount() const
  {
    return ldrCount;
  }
};

// include templates
#include "AutoLdrSpotDetectors.cpp"
