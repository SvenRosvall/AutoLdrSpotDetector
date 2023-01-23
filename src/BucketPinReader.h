#pragma once

#include "PinReader.h"

class BucketPinReader : public PinReader
{
  friend void ADConversionInterruptServiceRouting();
  struct Bucket
  {
    bool isRead = false;
    int count = 0;
    long sumValues = 0;
  };

  static Bucket buckets[8];

public:
  void setup() override;
  int readValue(int pin) override;
};

BucketPinReader & createBucketPinReader();
