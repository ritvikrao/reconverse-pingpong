//+pe <N> threads, each running a scheduler
#include "convcore.h"
#include "scheduler.h"
#include "CpvMacros.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <barrier>

int Cmi_npes;
int Cmi_argc;
int _Cmi_myrank; /* Normally zero; only 1 during SIGIO handling */
int _Cmi_mynode;
int _Cmi_numnodes;

// define variables (do we use cpv or cth here?)
CpvDeclare(CmiHandlerInfo *, CmiHandlerTable);
CpvStaticDeclare(int, CmiHandlerCount);
CpvStaticDeclare(int, CmiHandlerLocal);
CpvStaticDeclare(int, CmiHandlerGlobal);
CpvDeclare(int, CmiHandlerMax);

void *converseRunPe(void *args)
{
    // call cmi start function, pass args to it
    Cmi_startfn(Cmi_argc, Cmi_argv);

    // init state struct
    CmiInitState();

    // declare handler structs
    CpvDeclare(CmiHandlerInfo *, CmiHandlerTable);

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

CmiState
CmiGetState(int pe)
{
    // TODO get state of pe
    CmiState state;
    return state;
}

void CmiInitState()
{
    CmiState state = CmiGetState();
    state.pe = 0; // TOOD: get pe from thread info
    state.rank = 0;

    state.queue = ConverseQueue<CmiMessage>();

    // TODO: store state in some global array
}

int CmiMyPE()
{
    return CmiGetState().pe;
}

int CmiMyNode()
{
    return CmiGetState().rank;
}

int CmiMyNodeSize()
{
    return 1; // TODO: get node size
}

void CmiPushPE(int destPE, int messageSize, void *msg)
{
    // TODO: add message to PE-level queue
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
static void CmiExtendHandlerTable(int atLeastLen)
{
    int max = CpvAccess(CmiHandlerMax);
    int newmax = (atLeastLen + (atLeastLen >> 2) + 32);
    int bytes = max * sizeof(CmiHandlerInfo);
    int newbytes = newmax * sizeof(CmiHandlerInfo);
    CmiHandlerInfo *nu = (CmiHandlerInfo *)malloc(newbytes);
    CmiHandlerInfo *tab = CpvAccess(CmiHandlerTable);
    // _MEMCHECK(nu);
    if (tab)
    {
        memcpy(nu, tab, bytes);
    }
    memset(((char *)nu) + bytes, 0, (newbytes - bytes));
    free(tab);
    tab = nu;
    CpvAccess(CmiHandlerTable) = tab;
    CpvAccess(CmiHandlerMax) = newmax;
}
void CmiNumberHandler(int n, CmiHandler h)
{
    CmiHandlerInfo *tab;
    if (n >= CpvAccess(CmiHandlerMax))
        CmiExtendHandlerTable(n);
    tab = CpvAccess(CmiHandlerTable);
    tab[n].hdlr = (CmiHandlerEx)h; /* LIE!  This assumes extra pointer will be ignored!*/
    tab[n].userPtr = 0;
}

#define DIST_BETWEEN_HANDLERS 1
int CmiRegisterHandler(CmiHandler h)
{
    int Count = CpvAccess(CmiHandlerCount);
    CmiNumberHandler(Count, h);
    CpvAccess(CmiHandlerCount) = Count + DIST_BETWEEN_HANDLERS;
    return Count;
}

// NODE BARRIER
void CmiNodeBarrier(void)
{
    // static Barrier nodeBarrier(CmiMyNodeSize());
    // nodeBarrier.arrive_and_wait();

    // TODO: implement barrier
}