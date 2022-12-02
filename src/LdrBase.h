#pragma once

class LdrBase
{
public:
  virtual LdrState getState() const = 0;
  virtual int value() const = 0;
  virtual int getThreshold() const = 0;
};
