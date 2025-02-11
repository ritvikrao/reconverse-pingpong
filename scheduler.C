#include "scheduler.h"
#include "convcore.h"
#include "queue.h"
#include <thread>

void CsdScheduler()
{
    // get pthread level queue

    ConverseQueue<CmiMessage *> *queue = CmiGetQueue(CmiMyRank());

    while (CmiStopFlag() == 0)
    {
        if (!queue->empty())
        {
            // get next event (guaranteed to be there because only single consumer)
            CmiMessage *msg = queue->pop();

            // process event
            CmiMessageHeader header = msg->header;
            int handler = header.handlerId;

            // call handler
            CmiCallHandler(handler, msg);
        }

        // TODO: suspend? or spin?
    }
}

// TODO: implement CsdEnqueue/Dequeue (why are these necessary?)
