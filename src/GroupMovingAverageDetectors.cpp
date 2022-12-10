#include "GroupMovingAverageDetectors.h"
#include "GroupMovingAverageLDR.h"
#include "TimedStateDecider.h"

#include <Streaming.h>

GroupMovingAverageDetectors::GroupMovingAverageDetectors(SensorChangeAction & action,
                                                         const std::initializer_list<uint8_t> & il,
                                                         StateDecider::Factory const & deciderFactory)
  : AutoLdrSpotDetectors(action, il)
  , deciderFactory(deciderFactory)
{
}

GroupMovingAverageDetectors::GroupMovingAverageDetectors(SensorChangeAction & action,
                                                         const std::initializer_list<uint8_t> & il)
  : AutoLdrSpotDetectors(action, il)
  , deciderFactory(createTimedStateDeciderFactory())
{
}

void GroupMovingAverageDetectors::setup()
{
  AutoLdrSpotDetectors::setup();

  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    StateDecider * stateDecider = deciderFactory.create(ldrs[i]);
    ldrs[i].stateDecider = stateDecider;
  }
}

float GroupMovingAverageDetectors::calculateAvgOfDiffs() const
{
  float sumOfDiffs = 0;
  for (unsigned int i=0 ; i < ldrCount ; ++i)
  {
    sumOfDiffs += ldrs[i].movingDiffAverage;
  }
  return sumOfDiffs / ldrCount;
}

void GroupMovingAverageDetectors::update()
{
  allLdrs([](GroupMovingAverageLDR & ldr) { ldr.readValue(); });
  avgOfDiffs = calculateAvgOfDiffs();
  allLdrs([](GroupMovingAverageLDR & ldr) { ldr.updateState(); });
}

bool GroupMovingAverageDetectors::checkOtherLDRs(GroupMovingAverageLDR * thisLdr, LdrState checkedState)
{
  const int diffChangeThreshold = 50;

  // Check the trend (avgDiff) for the other LDRs.
  int sumOfDiffs = 0;
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].sensorPin == thisLdr->sensorPin)
      continue;
    sumOfDiffs += ldrs[i].movingDiffAverage;
  }
  DEBUG("checkOtherLdrs(this Pin=" << thisLdr->sensorPin << ", checkedState=" << checkedState
        << ") sumOfDiffs=" << sumOfDiffs << " ldrCount=" << ldrCount);
  switch (checkedState)
  {
    case COVERING:
      return sumOfDiffs / ((int) ldrCount - 1) > diffChangeThreshold;
    case OPENING:
      return sumOfDiffs / ((int) ldrCount - 1) < -diffChangeThreshold;
    default:
      Serial.println("Unknown state to check in GroupMovingAverageDetectors::checkOtherLDRs()");
      return false;
  }
}
