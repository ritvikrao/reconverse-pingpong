#include "converse.h"
#include <stdio.h>
#include <pthread.h>

CpvDeclare(int, test);

void ping_handler(void *vmsg)
{
  printf("PING HANDLER CALLED\n");
}

CmiStartFn mymain(int argc, char **argv)
{
  // CpvInitialize(int, test);
  // CpvAccess(test) = 42;

  printf("My PE is %d\n", CmiMyRank());

  int handlerId = CmiRegisterHandler(ping_handler);

  if (CmiMyRank() == 0 && CmiMyNodeSize() > 1)
  {
    // create a message
    CmiMessage *msg = new CmiMessage;
    msg->header.handlerId = handlerId;
    msg->header.messageSize = sizeof(CmiMessage);
    msg->header.destPE = 1;

    // TODO: why is this info passed within message an also separately
    int sendToPE = 1;

    // Send from my pe-i on node-0 to q+i on node-1
    CmiSyncSendAndFree(sendToPE, msg->header.messageSize, msg);
  }

  else if (CmiMyNodeSize() == 1)
  {
    printf("Only one node, send self test\n");
    // create a message
    CmiMessage *msg = new CmiMessage;
    msg->header.handlerId = handlerId;
    msg->header.messageSize = sizeof(CmiMessage);
    msg->header.destPE = 1;

    // TODO: why is this info passed within message an also separately
    int sendToPE = 0;

    // Send from my pe-i on node-0 to q+i on node-1
    CmiSyncSendAndFree(sendToPE, msg->header.messageSize, msg);
  }

  // printf("Answer to the Ultimate Question of Life, the Universe, and Everything: %d\n", CpvAccess(test));
  return 0;
}

int main(int argc, char **argv)
{
  ConverseInit(argc, argv, (CmiStartFn)mymain);
  return 0;
}