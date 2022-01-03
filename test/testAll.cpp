#include <map>
#include <string>
#include "testArduino.hpp"
#include "testThresholdDetectors.h"
#include "testMovingAverageDetectors.h"

std::map<std::string, void (*)()> suites = {
  {"Arduino", testArduino},
  {"ThresholdDetectors", testThresholdDetectors},
  {"MovingAverageDetectors", testMovingAverageDetectors},
};

int main(int argc, const char * const * argv)
{
  if (*++argv == nullptr)
  {
    for (auto const &i : suites)
    {
      i.second();
    }
  }
  else
  {
    while (const char * arg = *argv++)
    {
      suites[arg]();
    }
  }
}
