#pragma once

#include <initializer_list.h>

#include "LdrState.h"

template <class LDRT>
struct LDR;

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

  TransitionState areLdrsChanging(LdrState transitionState, LdrState finalState);
  void changeState(LdrState fromState, LdrState toState);
  void checkTransitions();

public:
  AutoLdrSpotDetectors(SensorChangeAction & action, const std::initializer_list<int> & il);
  void setThresholdLevel(int l);
  void setMovingAverageP(float p);

  void setup();
  void update();

  void allLdrs(void (*f)(LDR<LDRT> &));
  bool checkOtherLDRs(LDR<LDRT> * thisLdr, LdrState checkedState);
  void onChange(LDR<LDRT> * thisLdr, LdrState newState);

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
