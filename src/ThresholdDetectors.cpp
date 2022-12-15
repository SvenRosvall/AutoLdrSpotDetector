#include "ThresholdDetectors.h"
#include "ThresholdLDR.h"
#include "InstantStateDecider.h"

ThresholdDetectors::ThresholdDetectors(SensorChangeAction & action,
                                       const std::initializer_list<uint8_t> & il,
                                       StateDecider::Factory const & deciderFactory,
                                       int threshold)
  : AutoLdrSpotDetectors(action, il, deciderFactory)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].threshold = threshold;
  }
}

ThresholdDetectors::ThresholdDetectors(SensorChangeAction & action,
                                       const std::initializer_list<uint8_t> & il,
                                       int threshold)
  : AutoLdrSpotDetectors(action, il, createInstantStateDeciderFactory())
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
