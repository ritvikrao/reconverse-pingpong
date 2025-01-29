#ifndef CONVCORE_H
#define CONVCORE_H

#include "converse.h"
#include "queue.h"

static char **Cmi_argv;

static CmiStartFn Cmi_startfn;

void CmiStartThreads(char **argv);

void *converseRunPe(void *arg);

/*Cmi Functions*/
typedef struct Header
{
    int handlerId;
    int messageId;
    int messageSize;
    int destPE;
} CmiMessageHeader;

#define CmiMessageHeaderSize sizeof(CmiMessageHeader)

typedef struct CmiMessageStruct
{
    CmiMessageHeader header;
    char data[];
} CmiMessage;

typedef struct State
{
    int pe;
    int rank;
    ConverseQueue<CmiMessage> queue;

} CmiState;

// state relevant functionality
CmiState CmiGetState(void);
CmiState CmiGetState(int pe);
void CmiInitState();

// state getters
int CmiMyPE();
int CmiMyNode();

// message sending
void CmiPushPE(int destPE, int messageSize, void *msg);
void CmiSyncSendAndFree(int destPE, int messageSize, void *msg);

#endif