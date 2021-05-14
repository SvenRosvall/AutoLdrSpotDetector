#include <initializer_list.h>
#include "LDR.h"

class SensorChangeAction
{
public:
  virtual void onChange(int ldrIndex, bool covered) = 0;
};

enum TransitionState
{
  NOT_TRANSITIONING, TRANSITIONING, TRANSITIONED
};

class AutoLdrSpotDetectors
{
  SensorChangeAction & action;
  unsigned int ldrCount;
  LDR * ldrs;

  TransitionState areLdrsChanging(LdrState transitionState, LdrState finalState);
  void changeState(LdrState fromState, LdrState toState);
  void checkTransitions();

public:
  AutoLdrSpotDetectors(SensorChangeAction & action, std::initializer_list<int> il);
  void setup();
  void update();

  void allLdrs(void (*f)(LDR &));
  bool checkOtherLDRs(LDR * thisLdr, LdrState checkedState);
  void onChange(LDR * thisLdr, LdrState newState);

  void plotTitleAll();
  void plotTitleDetailed(int nLdr);
  void plotAll();
  void plotDetailed(int nLdr);

  LDR const * getLdrs() const
  {
    return ldrs;
  }
};
