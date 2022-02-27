#include "GroupMovingAverageDetectors.h"
#include "GroupMovingAverageLDR.h"

#include <Streaming.h>

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
      return sumOfDiffs / ((int) ldrCount - 1) > 100;
    case OPENING:
      return sumOfDiffs / ((int) ldrCount - 1) < -100;
    default:
      Serial.println("Unknown state to check in GroupMovingAverageDetectors::checkOtherLDRs()");
      return false;
  }
}
