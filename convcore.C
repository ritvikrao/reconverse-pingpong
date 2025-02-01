//+pe <N> threads, each running a scheduler
#include "convcore.h"
#include "scheduler.h"
#include "CpvMacros.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int Cmi_argc;
static char **Cmi_argv;
int Cmi_npes;

// define variables (do we use cpv or cth here?)
CpvDeclare(CmiHandlerInfo *, CmiHandlerTable);
CpvStaticDeclare(int, CmiHandlerCount);
CpvStaticDeclare(int, CmiHandlerLocal);
CpvStaticDeclare(int, CmiHandlerGlobal);
CpvDeclare(int, CmiHandlerMax);

thread_local CmiState *Cmi_state;

void *converseRunPe(void *args)
{
    // init state
    int pe = *(int *)args;
    CmiInitState(pe);

    // declare handler structs
    CpvDeclare(CmiHandlerInfo *, CmiHandlerTable);

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
static pthread_key_t Cmi_state_key = (pthread_key_t)(-1);

CmiState *
CmiGetState(void)
{
    return Cmi_state;
};

void CmiInitState(int pe)
{
    Cmi_state = new CmiState;
    Cmi_state->pe = pe;  // TODO: for now, pe is just thread index
    Cmi_state->node = 0; // TODO: get node

    ConverseQueue<CmiMessage> queue;
    Cmi_state->queue = queue;
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