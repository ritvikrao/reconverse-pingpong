#include "scheduler.h"
#include "convcore.h"
#include "queue.h"
#include <thread>

void CsdScheduler()
{
    // get pthread level queue

    ConverseQueue<void *> *queue = CmiGetQueue(CmiMyRank());

    while (CmiStopFlag() == 0)
    {
        if (!queue->empty())
        {
            // get next event (guaranteed to be there because only single consumer)
            void *msg = queue->pop();

            // process event
            CmiMessageHeader *header = (CmiMessageHeader *)msg;
            void *data = (void *)((char *)msg + CmiMessageHeaderSize);
            int handler = header->handlerId;

            // call handler
            CmiCallHandler(handler, msg);
        }

        // TODO: suspend? or spin?
    }
}

// TODO: implement CsdEnqueue/Dequeue (why are these necessary?)
