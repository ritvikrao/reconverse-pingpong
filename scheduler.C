#include "scheduler.h"
#include "convcore.h"
#include "queue.h"

void CsdScheduler()
{
    // get pthread level queue

    ConverseQueue<CmiMessage> queue = CmiGetState()->queue;

    while (true)
    {
        if (queue.empty())
        {
            // wait for new event
            // TODO: do we just spin? or is there a better way?
            continue;
        }

        // get next event
        CmiMessage message = queue.pop();

        // TODO: process event
        CmiMessageHeader header = message.header;
        int handler = header.handlerId;

        // CpvAccess(CmiHandlerTable)[handler].hdlr(message);
    }
}

// TODO: implement CsdEnqueue: how does a PE get another PEs state/queue?
