enum LdrState
{
  OPEN,
  COVERED,
  COVERING,
  OPENING
};

class Print;
Print & operator<<(Print & p, LdrState state);
