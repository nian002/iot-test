#include "func.h"

int fac(int nInput)
{
  if (nInput < 0)
  {
    return -1;
  }

  int nRev = 1;
  for (int i = 1; i <= nInput; ++i)
  {
    nRev *= i;
  }
  return nRev;
}