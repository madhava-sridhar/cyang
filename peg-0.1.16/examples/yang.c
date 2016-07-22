#include <stdio.h>
#include <stdlib.h>

//#define YY_DEBUG
#include "yang.leg.c"

int main()
{
  if  (!yang_parse()) {
    printf("Syntax error\n");
    return 1;
  }

  return 0;
}
