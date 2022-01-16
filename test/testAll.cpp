#include <map>
#include <string>
#include "testArduino.hpp"
#include "testThresholdDetectors.h"
#include "testMovingAverageDetectors.h"
#include "testGroupMovingAverageDetectors.h"

std::map<std::string, void (*)()> suites = {
  {"Arduino", testArduino},
  {"ThresholdDetectors", testThresholdDetectors},
  {"MovingAverageDetectors", testMovingAverageDetectors},
  {"GroupMovingAverageDetectors", testGroupMovingAverageDetectors},
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
