#ifndef CONVCORE_H
#define CONVCORE_H

#include "converse.h"

static char **Cmi_argv;

static CmiStartFn Cmi_startfn;
int Cmi_npes;
int Cmi_argc;

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

typedef struct Message
{
    CmiMessageHeader header;
    char data[];
} CmiMessage;

typedef struct State
{
    int pe;
    int rank;
} CmiState;

// state relevant functionality
CmiState CmiGetState(void);
void CmiInitState();

// state getters
int CmiMyPE();
int CmiMyNode();

// message sending
void CmiPushPE(int destPE, int messageSize, void *msg);
void CmiSyncSendAndFree(int destPE, int messageSize, void *msg);

#endif