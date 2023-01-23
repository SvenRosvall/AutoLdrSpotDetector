#pragma once

#include <Arduino.h>

#include "PinReader.h"

class SimplePinReader : public PinReader
{
public:
  int readValue(int pin) override
  {
    return analogRead(pin);
  }  
};

SimplePinReader & createSimplePinReader();
