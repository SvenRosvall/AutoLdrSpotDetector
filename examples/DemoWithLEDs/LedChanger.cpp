#include "LedChanger.h"
#include <Arduino.h>
#include <Streaming.h>

// Helper to get a list of LDR pins from LDR/LED pairs.
std::initializer_list<uint8_t> getLdrPins(std::initializer_list<LdrLedPair> il)
{
  uint8_t * ret = new uint8_t[il.size()];
  auto p = ret;
  for (auto e : il)
  {
    *p++ = e.ldrPin;
  }
  return std::initializer_list<uint8_t>(ret, il.size());
}

LedChanger::LedChanger(std::initializer_list<uint8_t> il)
{
  leds = new uint8_t[il.size()];
  auto p = leds;
  for (auto e : il)
  {
    *p++ = e;
    pinMode(e, OUTPUT);
    //Serial << "LedChanger with LED pin=" << e << endl;
  }
}

LedChanger::LedChanger(std::initializer_list<LdrLedPair> il)
{
  leds = new uint8_t[il.size()];
  auto p = leds;
  for (auto e : il)
  {
    *p++ = e.ledPin;
    pinMode(e.ledPin, OUTPUT);
    //Serial << "LedChanger with LED pin=" << e.ledPin << endl;
  }
}

void LedChanger::onChange(int ldrIndex, bool covered)
{
  digitalWrite(leds[ldrIndex], covered ? HIGH : LOW);
  //Serial << "LedChanger changing led=" << ldrIndex << " pin=" << leds[ldrIndex] << " to " << (covered ? "HIGH" : "LOW") << endl;
}
