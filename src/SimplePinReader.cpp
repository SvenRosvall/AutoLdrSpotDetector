#include "SimplePinReader.h"

SimplePinReader & createSimplePinReader()
{
  static SimplePinReader pinReader;
  return pinReader;
}
