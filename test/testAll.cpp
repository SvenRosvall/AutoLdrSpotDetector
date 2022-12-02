#include <map>
#include <string>
#include <iostream>
#include "testArduino.hpp"
#include "testThresholdDetectors.h"
#include "testAdjustingDetectors.h"
#include "testMovingAverageDetectors.h"
#include "testGroupMovingAverageDetectors.h"
#include "testGroupMovingAverageLDR.h"
#include "testInstantStateDecider.h"
#include "testTimedStateDecider.h"

std::map<std::string, void (*)()> suites = {
  {"Arduino", testArduino},
  {"InstantStateDecider", testInstantStateDecider},
  {"TimedStateDecider", testTimedStateDecider},
  {"ThresholdDetectors", testThresholdDetectors},
  {"AdjustingDetectors", testAdjustingDetectors},
  {"MovingAverageDetectors", testMovingAverageDetectors},
  {"GroupMovingAverageDetectors", testGroupMovingAverageDetectors},
  {"GroupMovingAverageLDR", testGroupMovingAverageLDR},
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
    bool needHelp = false;
    while (const char * arg = *argv++)
    {
      auto found = suites.find(arg);
      if (found != suites.end())
      {
        found->second();
      }
      else
      {
        std::cout << "Cannot find test suite '" << arg << "'" << std::endl;
        needHelp = true;
      }
    }
    if (needHelp)
    {
      std::cout << "The following test suites are available:" << std::endl;
      for (auto & suite : suites)
      {
        std::cout << "  " << suite.first << std::endl;
      }
    }
  }
}
