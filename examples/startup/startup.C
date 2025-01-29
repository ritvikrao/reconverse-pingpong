#include "reconverse.h"

CmiStartFn mymain(char *argv[])
{

}

int main(int argc,char *argv[])
{
  ConverseInit(argc,argv,(CmiStartFn)mymain,0,0);
  return 0;
}