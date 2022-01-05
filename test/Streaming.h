#include "Arduino.hpp"

Print & operator<<(Print & p, const char * s);
Print & operator<<(Print & p, int i);
Print & operator<<(Print & p, float f);
extern int endl;
