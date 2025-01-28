#include "scheduler.h"

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

void CmiSyncSendAndFree(int destNode, int destPE, int messageSize, void *msg)
{
    if (CmiMyNode() == destNode)
    {
        CmiPushPE(destPE, messageSize, msg);
    }
    else
    {
        // TODO: handle off node message send
    }
}
