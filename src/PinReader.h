#pragma once

class PinReader
{
public:
  virtual void setup()
  {}

  virtual int readValue(int pin) = 0;
};
