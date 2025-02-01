#ifndef CONVCORE_H
#define CONVCORE_H

#include "converse.h"
#include "queue.h"

typedef void (*CmiStartFn)(int argc, char **argv);
void ConverseInit(int argc, char **argv, CmiStartFn fn);

static CmiStartFn Cmi_startfn;

typedef struct Header
{
    int handlerId;
    int messageId;
    int messageSize;
    int destPE;
} CmiMessageHeader;

typedef struct CmiMessageStruct
{
    CmiMessageHeader header;
    char data[];
} CmiMessage;

// handler functionality
// TODO: what is CmiHandlerEx in old converse?

typedef void (*CmiHandler)(void *msg);
typedef void (*CmiHandlerEx)(void *msg, void *userPtr); // ignore for now

void CmiCallHandler(int handlerId, void *msg);

typedef struct HandlerInfo
{
    CmiHandler hdlr;
    void *userPtr;
} CmiHandlerInfo;

void CmiStartThreads(char **argv);

void *converseRunPe(void *arg);

/*Cmi Functions*/

typedef struct State
{
    int pe;
    int node;
    ConverseQueue<CmiMessage> *queue;

} CmiState;

// state relevant functionality
CmiState *CmiGetState(void);
void CmiInitState(int pe);

// state getters
int CmiMyPE();
int CmiMyNode();
int CmiMyNodeSize();

// message sending
void CmiPushPE(int destPE, int messageSize, void *msg);
void CmiSyncSendAndFree(int destPE, int messageSize, void *msg);

void CmiNodeBarrier(void);

#endif
