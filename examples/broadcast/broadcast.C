#include "converse.h"
#include <stdio.h>
#include <pthread.h>

CpvDeclare(int, exitHandlerId);

void stop_handler(void *vmsg)
{
  CsdExitScheduler();
}

void ping_handler(void *vmsg)
{
  printf("PING HANDLER CALLED ON PE %i\n", CmiMyPE());
  stop_handler(NULL);
}

CmiStartFn mymain(int argc, char **argv)
{
  printf("My PE is %d\n", CmiMyRank());

  int handlerId = CmiRegisterHandler(ping_handler);

  if (CmiMyPE() == 0)
  {
    // create a message
    CmiMessage *msg = (CmiMessage*) CmiAlloc(sizeof(CmiMessage));
    msg->header.handlerId = handlerId;
    msg->header.messageSize = sizeof(CmiMessage);
    
    CmiSyncBroadcastAllAndFree(msg->header.messageSize, msg);     
  }

  // printf("Answer to the Ultimate Question of Life, the Universe, and Everything: %d\n", CpvAccess(test));
  return 0;
}

int main(int argc, char **argv)
{
  ConverseInit(argc, argv, (CmiStartFn)mymain);
  return 0;
}