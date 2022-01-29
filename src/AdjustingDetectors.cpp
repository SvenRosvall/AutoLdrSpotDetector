#include "AdjustingDetectors.h"
#include "AdjustingLDR.h"

AdjustingDetectors::AdjustingDetectors(SensorChangeAction & action, const std::initializer_list<int> & il, int thresholdLevel)
  : AutoLdrSpotDetectors(action, il)
  , thresholdLevel(thresholdLevel)
{
}

void AdjustingDetectors::update()
{
  allLdrs([](AdjustingLDR & ldr) { ldr.readValue(); });
  allLdrs([](AdjustingLDR & ldr) { ldr.updateState(); });
}
