#pragma once

class LdrBase
{
public:
  virtual int value() const = 0;
  virtual int getThreshold() const = 0;
};
