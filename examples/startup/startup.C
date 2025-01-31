#include "converse.h"
#include <stdio.h>
#include <pthread.h>

CpvDeclare(int, test_mype);

void ping_handler(void *vmsg)
{
  printf("Ping handler called\n");
}

CmiStartFn mymain(int argc, char **argv)
{
  printf("Calling main, argc=%d\n", argc);
  CpvInitialize(int, test_mype);
  CpvAccess(test_mype) = CmiMyPE();

  printf("My PE is %d\n", CpvAccess(test_mype));
  return 0;
}

int main(int argc, char **argv)
{
  ConverseInit(argc, argv, (CmiStartFn)mymain);
  return 0;
}