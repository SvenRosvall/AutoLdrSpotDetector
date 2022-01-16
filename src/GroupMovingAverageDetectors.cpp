#include "GroupMovingAverageDetectors.h"
#include "GroupMovingAverageLDR.h"

void GroupMovingAverageDetectors::setThresholdLevel(int l)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setThresholdLevel(l);
  }
}

void GroupMovingAverageDetectors::setMovingAverageP(float p)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setMovingAverageP(p);
  }
}

void GroupMovingAverageDetectors::setMovingDiffAverageP(float p)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setMovingDiffAverageP(p);
  }
}

void GroupMovingAverageDetectors::setSelfDiffRatio(float r)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setSelfDiffRatio(r);
  }
}

void GroupMovingAverageDetectors::setChangeInterval(unsigned int interval)
{
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    ldrs[i].setChangeInterval(interval);
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

