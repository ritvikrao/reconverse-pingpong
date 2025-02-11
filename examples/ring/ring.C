#include "converse.h"
#include <stdio.h>
#include <pthread.h>

CpvDeclare(int, test);
CpvDeclare(int, exitHandlerId);

int ping_handlerID;
int payloadSize = 1 * sizeof(int);

void stop_handler(void *vmsg)
{
  CsdExitScheduler();
}

void ping_handler(void *vmsg)
{
  CmiMessage *msg = (CmiMessage *)vmsg;
  printf("PE %d pinged in ring with index %d.\n", CmiMyRank(), msg->data[0]);

  if (CmiMyRank() != CmiMyNodeSize() - 1)
  {
    CmiMessage *newmsg = (CmiMessage *)CmiAlloc(sizeof(CmiMessage) + payloadSize);
    newmsg->header.handlerId = ping_handlerID;
    newmsg->header.messageSize = sizeof(CmiMessage) + payloadSize;
    newmsg->header.destPE = CmiMyRank() + 1;

    newmsg->data[0] = msg->data[0] + 1;
    printf("PE %d sending to PE %d with data %d\n", CmiMyRank(), CmiMyRank() + 1, newmsg->data[0]);

    CmiSyncSendAndFree(CmiMyRank() + 1, newmsg->header.messageSize, newmsg);
  }
  else
  {
    CmiMessage *msg = (CmiMessage *)CmiAlloc(sizeof(CmiMessage));
    msg->header.handlerId = CpvAccess(exitHandlerId);
    msg->header.messageSize = sizeof(CmiMessage);
    CmiSyncBroadcastAllAndFree(msg->header.messageSize, msg);
  }
}

CmiStartFn mymain(int argc, char **argv)
{
  CpvInitialize(int, test);
  CpvAccess(test) = 42;

  ping_handlerID = CmiRegisterHandler(ping_handler);

  if (CmiMyRank() == 0)
  {
    // create a message
    CmiMessage *msg = (CmiMessage *)CmiAlloc(sizeof(CmiMessage) + payloadSize);
    msg->header.handlerId = ping_handlerID;
    msg->header.messageSize = sizeof(CmiMessage) + payloadSize;
    msg->header.destPE = 0;
    msg->data[0] = 0;

    // TODO: why is this info passed within message an also separately
    int sendToPE = 0;

    // Send from my pe-i on node-0 to q+i on node-1
    CmiSyncSendAndFree(sendToPE, msg->header.messageSize, msg);
  }

  CpvInitialize(int, exitHandlerId);
  CpvAccess(exitHandlerId) = CmiRegisterHandler(stop_handler);

  // printf("Answer to the Ultimate Question of Life, the Universe, and Everything: %d\n", CpvAccess(test));
  return 0;
}

int main(int argc, char **argv)
{
  ConverseInit(argc, argv, (CmiStartFn)mymain);
  return 0;
}