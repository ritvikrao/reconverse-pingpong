#include "reconverse.h"
#include <stdio.h>
#include <pthread.h>

void* mymain(void* argv)
{
  printf("Calling main\n");
  return 0;
}

int main(int argc,char **argv)
{
  ConverseInit(argc,argv,(CmiStartFn)mymain);
  return 0;
}