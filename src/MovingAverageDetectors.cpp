#include "MovingAverageDetectors.h"
#include "MovingAverageLDR.h"

bool MovingAverageDetectors::checkOtherLDRs(MovingAverageLDR * thisLdr, LdrState checkedState)
{
  unsigned int countInState = 0;
  for (unsigned int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].sensorPin == thisLdr->sensorPin)
      continue;
    if (ldrs[i].state == checkedState)
      ++countInState;
  }
  return countInState > ldrCount / 2;
}
