#include "AdjustingDetectors.h"
#include "AdjustingLDR.h"
#include "TimedStateDecider.h"


AdjustingDetectors::AdjustingDetectors(SensorChangeAction & action,
                                       const std::initializer_list<uint8_t> & il,
                                       StateDecider::Factory const & deciderFactory,
                                       int thresholdLevel)
  : AutoLdrSpotDetectors(action, il)
  , deciderFactory(deciderFactory)
  , thresholdLevel(thresholdLevel)
{
}

AdjustingDetectors::AdjustingDetectors(SensorChangeAction & action,
                                       const std::initializer_list<uint8_t> & il,
                                       int thresholdLevel)
  : AutoLdrSpotDetectors(action, il)
  , deciderFactory(createTimedStateDeciderFactory())
  , thresholdLevel(thresholdLevel)
{
}

void AdjustingDetectors::setup()
{
  AutoLdrSpotDetectors::setup();

  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].stateDecider = deciderFactory.create(ldrs[i]);
  }
}

void AdjustingDetectors::update()
{
  allLdrs([](AdjustingLDR & ldr) { ldr.readValue(); });
  allLdrs([](AdjustingLDR & ldr) { ldr.updateState(); });
}
