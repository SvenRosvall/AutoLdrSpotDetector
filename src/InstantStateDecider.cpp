#include "InstantStateDecider.h"

InstantStateDecider::Factory const & createInstantStateDeciderFactory()
{
  static InstantStateDecider::Factory factory;
  return factory;
}
