#include <map>
#include <string>
#include "testArduino.hpp"
#include "testMovingAverageDetectors.h"

std::map<std::string, void (*)()> suites = {
  {"Arduino", testArduino},
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
