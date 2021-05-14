#include <map>
#include <string>
#include "testArduino.hpp"
#include "testAutoLdrSpotDetectors.h"

std::map<std::string, void (*)()> suites = {
  {"Arduino", testArduino},
  {"AutoLdrSpotDetectors", testAutoLdrSpotDetectors},
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
