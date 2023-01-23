#pragma once

#include <initializer_list.h>
#include <Arduino.h>

#include "LdrState.h"
#include "StateDecider.h"
#include "PinReader.h"

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
  StateDecider::Factory const & deciderFactory;
  PinReader & pinReader;

  void changeState(LdrState fromState, LdrState toState);

public:
  AutoLdrSpotDetectors(SensorChangeAction & action,
                       const std::initializer_list<uint8_t> & il,
                       StateDecider::Factory const & deciderFactory);
  AutoLdrSpotDetectors(SensorChangeAction & action,
                       const std::initializer_list<uint8_t> & il,
                       StateDecider::Factory const & deciderFactory,
                       PinReader & pinReader);

  void setup();
  virtual void update() = 0;

  int readValue(int pin)
  {
    return pinReader.readValue(pin);
  }

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
