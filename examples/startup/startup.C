#include "converse.h"
#include <stdio.h>
#include <pthread.h>

CmiStartFn mymain(int argc, char **argv)
{
  printf("Calling main, argc=%d\n", argc);
  return 0;
}

int main(int argc, char **argv)
{
  ConverseInit(argc, argv, (CmiStartFn)mymain);
  return 0;
}