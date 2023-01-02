#include "GroupMovingAverageDetectors.h"
#include "GroupMovingAverageLDR.h"
#include "TimedStateDecider.h"

#include <Streaming.h>

GroupMovingAverageDetectors::GroupMovingAverageDetectors(SensorChangeAction & action,
                                                         const std::initializer_list<uint8_t> & il,
                                                         StateDecider::Factory const & deciderFactory)
  : AutoLdrSpotDetectors(action, il, deciderFactory)
{
}

GroupMovingAverageDetectors::GroupMovingAverageDetectors(SensorChangeAction & action,
                                                         const std::initializer_list<uint8_t> & il)
  : AutoLdrSpotDetectors(action, il, createTimedStateDeciderFactory())
{
}

float GroupMovingAverageDetectors::calculateAvgOfDiffs() const
{
  float sumOfDiffs = 0;
  for (unsigned int i=0 ; i < ldrCount ; ++i)
  {
    sumOfDiffs += ldrs[i].lastValue - ldrs[i].movingAverage;
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
  const int diffChangeThreshold = 100;

  const float S = getThresholdScaling(); // How much to take scaling into account. 1.0 => totally. 0.0 => Use value level as is.

  // Check the trend (avgDiff) for the other LDRs.
  int sumOfDiffs = 0;
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].sensorPin == thisLdr->sensorPin)
      continue;

    float scale = (1-S) + S * (1024 - ldrs[i].movingAverage) / 1024.0f;
    sumOfDiffs += ldrs[i].movingDiffAverage / scale;
  }
  thisLdr->setAvgOfOtherDiffs(sumOfDiffs / ((int) ldrCount - 1));

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
