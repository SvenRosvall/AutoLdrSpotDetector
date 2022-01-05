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
void AutoLdrSpotDetectors<LDRT>::changeState(LdrState fromState, LdrState toState)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].state == fromState)
      ldrs[i].state = toState;
  }
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
void AutoLdrSpotDetectors<LDRT>::setup()
{
  allLdrs([](LDRT & ldr) { ldr.setup(); });
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
