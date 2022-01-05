#include "ThresholdDetectors.h"
#include "ThresholdLDR.h"

ThresholdDetectors::ThresholdDetectors(SensorChangeAction & action, const std::initializer_list<int> & il, int threshold)
  : AutoLdrSpotDetectors(action, il)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].threshold = threshold;
  }
}

void ThresholdDetectors::update()
{
  allLdrs([](ThresholdLDR & ldr) { ldr.readValue(); });
  allLdrs([](ThresholdLDR & ldr) { ldr.updateState(); });
}
