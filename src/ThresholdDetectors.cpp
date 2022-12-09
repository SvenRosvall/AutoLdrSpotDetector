#include "ThresholdDetectors.h"
#include "ThresholdLDR.h"
#include "InstantStateDecider.h"

ThresholdDetectors::ThresholdDetectors(SensorChangeAction & action, const std::initializer_list<uint8_t> & il, int threshold)
  : AutoLdrSpotDetectors(action, il)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].threshold = threshold;
  }
}

void ThresholdDetectors::setup()
{
  AutoLdrSpotDetectors::setup();

  InstantStateDecider::Factory * deciderFactory = new InstantStateDecider::Factory;
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].stateDecider = deciderFactory->create(ldrs[i]);
  }
}

void ThresholdDetectors::update()
{
  allLdrs([](ThresholdLDR & ldr) { ldr.readValue(); });
  allLdrs([](ThresholdLDR & ldr) { ldr.updateState(); });
}
