//+pe <N> threads, each running a scheduler
#include "convcore.h"
#include "scheduler.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *converseRunPe(void *args)
{
    // call cmi start function, pass args to it
    Cmi_startfn(Cmi_argc, Cmi_argv);
    // call scheduler
    CsdScheduler();
    return NULL;
}

void CmiStartThreads()
{
    pthread_t threadId[Cmi_npes];
    for (int i = 0; i < Cmi_npes; i++)
    {
        pthread_create(&threadId[i], NULL, converseRunPe, NULL);
    }
    for (int i = 0; i < Cmi_npes; i++)
    {
        pthread_join(threadId[i], NULL);
    }
}

// argument form: ./prog +pe <N>
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

CmiState
CmiGetState(void)
{
    CmiState state; // TODO: get current pe state
    return state;
};

void CmiInitState()
{
    CmiState state = CmiGetState();
    state.pe = 0; // TOOD: get pe from thread info
    state.rank = 0;

    // TODO: store state in some global array indexed by thread id
}

int CmiMyPE()
{
    return CmiGetState().pe;
}

int CmiMyNode()
{
    return CmiGetState().rank;
}

void CmiPushPE(int destPE, int messageSize, void *msg)
{
    // TODO: add message to node-level queue
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