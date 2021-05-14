#include <Streaming.h>
#include "LdrState.h"

Print & operator<<(Print & p, LdrState state)
{
  switch (state)
  {
    case OPEN:
      p << "OPEN";
      break;
    case COVERED:
      p << "COVERED";
      break;
    case COVERING:
      p << "COVERING";
      break;
    case OPENING:
      p << "OPENING";
      break;
  }
  return p;
}
