#include <Streaming.h>
#include <limits.h>

// Choose what set of output is wanted.
//#define PLOT_ALL_VALUES
#define PLOT_DETAILS
#define PRINT_DEBUG

#ifdef PRINT_DEBUG
#define DEBUG(S) Serial << S << endl
#else
#define DEBUG(S)
#endif

// Tuning parameters
const int INTERVAL = 100; // ms
const float P = 0.10f;  // for moving average
const int THRESHOLD = 50;

enum LdrState
{
  OPEN,
  COVERED,
  COVERING,
  OPENING
};

enum TransitionState
{
  NOT_TRANSITIONING, TRANSITIONING, TRANSITIONED
};

struct LDR
{
  int sensorPin;
  int ledPin;
  int lastValue;
  float movingAverage = -1;
  LdrState state;
  int threshold = -1;
  int oldThreshold = -1;
  
  LDR(int sensorPin, int ledPin)
    : sensorPin(sensorPin), ledPin(ledPin), state(OPEN)
  {
    pinMode(sensorPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
  }

  void init()
  {
    lastValue = analogRead(sensorPin);
    movingAverage = lastValue;
    updateThreshold();
  }

  void readValue()
  {
    lastValue = analogRead(sensorPin);
    movingAverage = P * lastValue + (1 - P) * movingAverage;
    updateThreshold();
  }

  int value() const
  {
    return lastValue;
  }

  void updateThreshold();
  void updateState();

  bool checkOtherLDRs(LDR * thisLdr, LdrState checkedState);

  Print & printTitle(Print & p) const;
  Print & printValue(Print & p) const;
  Print & printValue() const;
};


void LDR::updateThreshold()
{
  switch (state)
  {
    case OPEN:
      threshold = movingAverage + THRESHOLD;
      oldThreshold = threshold;
      break;
    case COVERING:
      threshold = movingAverage + THRESHOLD;
      break;
    case COVERED:
      threshold = movingAverage - THRESHOLD;
      oldThreshold = threshold;
      break;
    case OPENING:
      threshold = movingAverage - THRESHOLD;
      break;
  }
}

void LDR::updateState()
{
  switch (state)
  {
    case OPEN:
      if (lastValue > threshold)
      {
        state = COVERING;
        DEBUG("LDR A" << sensorPin-A0 << " change to covering.");
      }
      break;
    case COVERING:
      if (lastValue < movingAverage - THRESHOLD)
      {
        state = OPEN;
        DEBUG("LDR A" << sensorPin-A0 << " change back to open.");
      }
      else if (movingAverage > oldThreshold && !checkOtherLDRs(this, COVERING))
      {
        state = COVERED;
        digitalWrite(ledPin, HIGH);
        DEBUG("LDR A" << sensorPin-A0 << " is covered, turn on led " << ledPin);
      }
      break;
    case COVERED:
      if (lastValue < threshold)
      {
        state = OPENING;
        DEBUG("LDR A" << sensorPin-A0 << " change to opening.");
      }
      break;
    case OPENING:
      if (lastValue > movingAverage + THRESHOLD)
      {
        state = COVERED;
        DEBUG("LDR A" << sensorPin-A0 << " change back to covered.");
      }
      else if (movingAverage < oldThreshold && !checkOtherLDRs(this, OPENING))
      {
        state = OPEN;
        digitalWrite(ledPin, LOW);
        DEBUG("LDR A" << sensorPin-A0 << " is open, turn off led " << ledPin << endl);
      }
      break;
  }
}

Print & LDR::printTitle(Print & p) const
{
#ifdef PLOT_DETAILS
  p << " valA" << sensorPin-A0 << " avgA" << sensorPin-A0 << " thresholdA" << sensorPin-A0;
#endif
#ifdef PLOT_ALL_VALUES
  p << " valA" << sensorPin-A0;
#endif
  return p;
}
Print & LDR::printValue(Print & p) const
{
#ifdef PLOT_DETAILS
  p << " " << lastValue << " " << movingAverage << " " << threshold;
#endif
#ifdef PLOT_ALL_VALUES
  p << " " << lastValue;
#endif
  return p;
}

Print & LDR::printValue() const
{
  printValue(Serial);
}

Print & operator<<(Print & p, LDR const & ldr)
{
  return ldr.printValue(p);
}

LDR ldrs[] = { LDR(A0, 10), LDR(A1, 9), LDR(A2, 8), LDR(A3, 7), LDR(A4, 6), LDR(A5, 5) };

bool LDR::checkOtherLDRs(LDR * thisLdr, LdrState checkedState)
{
  int ldrCount = sizeof(ldrs) / sizeof(ldrs[0]);
  int countInState = 0;
  for (int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].sensorPin == thisLdr->sensorPin)
      continue;
    if (ldrs[i].state == checkedState)
      ++countInState;
  }
  return countInState > ldrCount / 2;
}

void allLdrs(void (*f)(LDR &))
{
  for (int i = 0 ; i < sizeof(ldrs) / sizeof(ldrs[0]) ; ++i)
  {
    f(ldrs[i]);
  }
}

Print & operator<<(Print & p, LDR ldrs[])
{
  allLdrs([](LDR & ldr){ ldr.printValue(); }); // invalid user-defined conversion from 'operator<<(Print&, LDR*)::<lambda(LDR&)>' to 'void (*)(LDR&)'
//allLdrs([&](LDR & ldr){ ldr.print(Serial); }); //                cannot convert 'operator<<(Print&, LDR*)::<lambda(LDR&)>' to 'void (*)(LDR&)' for argument '1' to 'void allLdrs(void (*)(LDR&))'
  return p;
}

void setup() {
  digitalWrite(13, HIGH);
  Serial.begin(115200);
  // DEBUG("Setup()");

  allLdrs([](LDR & ldr) { ldr.init(); });

  // DEBUG("Start : " << ldrs);
#ifdef PLOT_DETAILS
  ldrs[0].printTitle(Serial);
  ldrs[1].printTitle(Serial);
  Serial << endl;
#endif
#ifdef PLOT_ALL_VALUES
  allLdrs([](LDR & ldr) { ldr.printTitle(Serial); });
  Serial << endl;
#endif
}

TransitionState areLdrsChanging(LdrState transitionState, LdrState altState)
{
  int countTransitioning = 0;
  int countTransitioned = 0;
  int countAltState = 0;
  const int ldrCount = sizeof(ldrs) / sizeof(ldrs[0]);
  for (int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].state == transitionState)
    {
      ++countTransitioning;
      if ((transitionState == COVERING )
        ? (ldrs[i].threshold > ldrs[i].lastValue)
        : (ldrs[i].threshold < ldrs[i].lastValue))
      {
        ++countTransitioned;
      }
    }
    if (ldrs[i].state == altState)
      ++countAltState;
  }
  DEBUG("Found " << countTransitioning << " transitioning, " 
    << countTransitioned << " transitioned, "
    << countAltState << " in alt state");
  if (countTransitioning == 0)
    return NOT_TRANSITIONING;
  if (countTransitioning + countAltState != ldrCount)
    return NOT_TRANSITIONING;
  if (countTransitioned == countTransitioning)
    return TRANSITIONED;
  else
    return TRANSITIONING;
}

void changeState(LdrState fromState, LdrState toState)
{
  const int ldrCount = sizeof(ldrs) / sizeof(ldrs[0]);
  for (int i = 0 ; i < ldrCount ; ++i)
  {
    if (ldrs[i].state == fromState)
      ldrs[i].state = toState;
  }
}

int transitionCount = 0;
void checkTransitions()
{
  TransitionState transitionState = areLdrsChanging(COVERING, COVERED);
  if (transitionState == TRANSITIONING || transitionState == TRANSITIONED)
  {
    DEBUG("Transitioning COVERING");
    if (transitionState == TRANSITIONED)
    {
      DEBUG("Transition time over, change to OPEN");
      changeState(COVERING, OPEN);
    }
  }
  else
  {
    transitionState = areLdrsChanging(COVERING, COVERED);
    if (transitionState == TRANSITIONING || transitionState == TRANSITIONED)
    {
      DEBUG("Transitioning OPENING");
      if (transitionState == TRANSITIONED)
      {
        DEBUG("Transition time over, change to OPEN");
        changeState(OPENING, COVERED);
      }
    }
    else
    {
      transitionCount = 0;
    }
  }
}

long lastMillis = millis();
void loop() {
  long startMillis = millis();
  allLdrs([](LDR & ldr) { ldr.readValue(); });
  checkTransitions();
  allLdrs([](LDR & ldr) { ldr.updateState(); });

  long now = millis();
#ifdef PLOT_DETAILS
  Serial // << _WIDTH(now-lastMillis,3)
         // << _WIDTH(now-startMillis,3)
         << ldrs[0]
         << ldrs[1]
         << endl;
#endif
#ifdef PLOT_ALL_VALUES
  allLdrs([](LDR & ldr) { ldr.printValue(Serial); });
  Serial << endl;
#endif
  
  lastMillis = now;
  delay(INTERVAL);
}