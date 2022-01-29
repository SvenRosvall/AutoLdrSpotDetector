#include "AdjustingLDR.h"
#include "AdjustingDetectors.h"

void AdjustingLDR::setup()
{
  LDR<AdjustingLDR, AdjustingDetectors>::setup();
  threshold = lastValue + parent->getThresholdLevel(); // Add threshold as we know that we are starting as OPEN.
  timer = millis() + 200;
}

void AdjustingLDR::updateState()
{
  //  threshold = lastValue + (state == OPEN ? threshold : -threshold);
  switch (state)
  {
  case OPEN:
    if (lastValue > threshold)
    {
      state = COVERING;
      timer = millis() + 500;
    }
    else
    {
      if (timer < millis())
      {
        if (lastValue + parent->getThresholdLevel() > threshold)
        {
          ++threshold;
        }
        else
        {
          --threshold;
        }
        timer = millis() + 200;
      }
    }
    break;

  case COVERING:
    if (lastValue < threshold)
    {
      state = OPEN;
      timer = millis();
    }
    else if (timer < millis())
    {
      state = COVERED;
      threshold = lastValue - parent->getThresholdLevel();
      parent->onChange(this, COVERED);
    }
    break;
    
  case COVERED:
    if (lastValue < threshold)
    {
      state = OPENING;
      timer = millis() + 500;
    }
    else
    {
      if (timer < millis())
      {
        if (lastValue - parent->getThresholdLevel() > threshold)
        {
          ++threshold;
        }
        else
        {
          --threshold;
        }
        timer = millis() + 200;
      }
    }
    break;

  case OPENING:
    if (lastValue > threshold)
    {
      state = COVERED;
      timer = millis();
    }
    else if (timer < millis())
    {
      state = OPEN;
      threshold = lastValue + parent->getThresholdLevel();
      parent->onChange(this, OPEN);
    }
    break;
  }
}
