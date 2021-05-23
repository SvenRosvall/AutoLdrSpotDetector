// Action to change an LED when a corresponding LDR changes.

#include <AutoLdrSpotDetectors.h>
#include <initializer_list.h>

struct LdrLedPair
{
  int ldrPin;
  int ledPin;
};

class LedChanger : public SensorChangeAction
{
  int * leds;
public:
  LedChanger(std::initializer_list<int> il);
  LedChanger(std::initializer_list<LdrLedPair> il);
  void onChange(int ldrIndex, bool covered);
};

// Helper to get a list of LDR pins from LDR/LED pairs.
std::initializer_list<int> getLdrPins(std::initializer_list<LdrLedPair> il);
