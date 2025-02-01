#include "scheduler.h"
#include "convcore.h"
#include "queue.h"
#include <thread>

void CsdScheduler()
{
    // get pthread level queue
    printf("DEBUG!! Starting scheduler\n");

    ConverseQueue<CmiMessage> queue = CmiGetState()->queue;

    while (true)
    {

        if (!queue.empty())
        {

            printf("Processing event\n");
            // get next event
            CmiMessage message = queue.pop();

            // TODO: process event
            CmiMessageHeader header = message.header;
            int handler = header.handlerId;

            CmiCallHandler(handler, message.data);

            // CpvAccess(CmiHandlerTable)[handler].hdlr(message);
        }
    }
}

// TODO: implement CsdEnqueue: how does a PE get another PEs state/queue?
