#include "converse.h"
#include <stdio.h>
#include <pthread.h>

CpvDeclare(int, test);

void ping_handler(void *vmsg)
{
  printf("Ping handler called\n");
}

CmiStartFn mymain(int argc, char **argv)
{
  CpvInitialize(int, test);
  CpvAccess(test) = 42;

  printf("My PE is %d\n", CmiMyPE());
  printf("Answer to the Ultimate Question of Life, the Universe, and Everything: %d\n", CpvAccess(test));
  return 0;
}

int main(int argc, char **argv)
{
  ConverseInit(argc, argv, (CmiStartFn)mymain);
  return 0;
}