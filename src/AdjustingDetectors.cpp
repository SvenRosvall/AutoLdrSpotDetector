#include "AdjustingDetectors.h"
#include "AdjustingLDR.h"
#include "TimedStateDecider.h"


AdjustingDetectors::AdjustingDetectors(SensorChangeAction & action,
                                       const std::initializer_list<uint8_t> & il,
                                       StateDecider::Factory const & deciderFactory,
                                       int thresholdLevel)
  : AutoLdrSpotDetectors(action, il, deciderFactory)
  , thresholdLevel(thresholdLevel)
{
}

AdjustingDetectors::AdjustingDetectors(SensorChangeAction & action,
                                       const std::initializer_list<uint8_t> & il,
                                       int thresholdLevel)
  : AutoLdrSpotDetectors(action, il, createTimedStateDeciderFactory())
  , thresholdLevel(thresholdLevel)
{
}

void AdjustingDetectors::update()
{
  allLdrs([](AdjustingLDR & ldr) { ldr.readValue(); });
  allLdrs([](AdjustingLDR & ldr) { ldr.updateState(); });
}
