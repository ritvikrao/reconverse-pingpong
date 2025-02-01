//+pe <N> threads, each running a scheduler
#include "convcore.h"
#include "scheduler.h"
#include "barrier.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// GLOBALS
int Cmi_argc;
static char **Cmi_argv;
int Cmi_npes;

// PE LOCALS that need global access sometimes
CmiState **Cmi_states; // array of state pointers

// PE LOCALS
thread_local int CmiHandlerCount = 0;
thread_local int CmiHandlerMax = 10;

// TODO: this should be shared global var
thread_local std::vector<CmiHandlerInfo> CmiHandlerTable;
// TODO: change to cmi_myrank
thread_local int Cmi_pe; // store my state alone

// TODO: padding for all these thread_locals and cmistates?

void CmiCallHandler(int handler, void *msg)
{
    CmiHandlerTable[handler].hdlr(msg);
}

void *converseRunPe(void *args)
{
    // init state
    int pe = *(int *)args;
    CmiInitState(pe);

    // call initial function and start scheduler
    Cmi_startfn(Cmi_argc, Cmi_argv);

    CsdScheduler();

    return NULL;
}

void CmiStartThreads()
{
    pthread_t threadId[Cmi_npes];

    // TODO: how to get enumerated pe nums for each thread?
    // this would be much cleaner with std::threads
    int threadPeNums[Cmi_npes];

    // allocate state array
    Cmi_states = (CmiState **)malloc(sizeof(CmiState *) * Cmi_npes);

    for (int i = 0; i < Cmi_npes; i++)
    {
        threadPeNums[i] = i;
        pthread_create(&threadId[i], NULL, converseRunPe, &threadPeNums[i]);
    }
    for (int i = 0; i < Cmi_npes; i++)
    {
        pthread_join(threadId[i], NULL);
    }
}

// argument form: ./prog +pe <N>
// TODO: this function need error checking
void ConverseInit(int argc, char **argv, CmiStartFn fn)
{

    Cmi_npes = atoi(argv[2]);
    // int plusPSet = CmiGetArgInt(argv,"+pe",&Cmi_npes);

    // NOTE: calling CmiNumPes() here it sometimes returns zero
    printf("Charm++> Running in SMP mode: %d processes\n", Cmi_npes);

    Cmi_argc = argc;
    Cmi_argv = (char **)malloc(sizeof(char *) * (argc + 1));
    int i;
    for (i = 0; i <= argc; i++)
        Cmi_argv[i] = argv[i];
    Cmi_startfn = fn;

    CmiStartThreads();
    free(Cmi_argv);
}

// CMI STATE
CmiState *
CmiGetState(void)
{
    return Cmi_states[Cmi_pe];
};

CmiState *
CmiGetState(int pe)
{
    return Cmi_states[pe];
};

void CmiInitState(int pe)
{
    CmiState *Cmi_state = new CmiState;
    Cmi_state->pe = pe;  // TODO: for now, pe is just thread index
    Cmi_state->node = 0; // TODO: get node

    Cmi_state->queue = new ConverseQueue<CmiMessage>();

    Cmi_states[pe] = Cmi_state;
    Cmi_pe = pe;
}

int CmiMyPE()
{
    return CmiGetState()->pe;
}

int CmiMyNode()
{
    return CmiGetState()->node;
}

int CmiMyNodeSize()
{
    return Cmi_npes; // TODO: get node size
}

void CmiPushPE(int destPE, int messageSize, void *msg)
{
    ConverseQueue<CmiMessage> *queue = CmiGetState(destPE)->queue;
    queue->push(*(CmiMessage *)msg);
}

void CmiSyncSendAndFree(int destPE, int messageSize, void *msg)
{
    int destNode = 0; // TODO:
    if (CmiMyNode() == destNode)
    {
        CmiPushPE(destPE, messageSize, msg);
    }
    else
    {
        // TODO: handle off node message send
    }
}

// HANDLER TOOLS
#define DIST_BETWEEN_HANDLERS 1
int CmiRegisterHandler(CmiHandler h)
{
    // add handler to vector
    CmiHandlerTable.push_back({h, nullptr});
    return CmiHandlerTable.size() - 1;
}

void CmiNodeBarrier(void)
{
    static Barrier nodeBarrier(CmiMyNodeSize());
    nodeBarrier.wait(); // TODO: this may be broken...
}