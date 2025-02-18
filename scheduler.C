#include "scheduler.h"
#include "convcore.h"
#include "queue.h"
#include <thread>

void CsdScheduler()
{
    // get pthread level queue

    ConverseQueue<void *> *queue = CmiGetQueue(CmiMyRank());
    ConverseNodeQueue<void *> *nodeQueue = CmiGetNodeQueue();

    while (CmiStopFlag() == 0)
    {
        
        CcdRaiseCondition(CcdSCHEDLOOP);

        // poll node queue
        if (!nodeQueue->empty())
        {
            // get next event (guaranteed to be there because only single consumer)
            CmiAcquireNodeQueueLock();
            if (!nodeQueue->empty())
            {
                void *msg = nodeQueue->pop();

                // process event
                CmiMessageHeader *header = (CmiMessageHeader *)msg;
                void *data = (void *)((char *)msg + CmiMsgHeaderSizeBytes);
                int handler = header->handlerId;

                // call handler
                CmiCallHandler(handler, data);

            }
            CmiReleaseNodeQueueLock();
        }

        // poll thread queue
        else if (!queue->empty())
        {
            // get next event (guaranteed to be there because only single consumer)
            void *msg = queue->pop();

            // process event
            CmiMessageHeader *header = (CmiMessageHeader *)msg;
            void *data = (void *)((char *)msg + CmiMsgHeaderSizeBytes);
            int handler = header->handlerId;

            // call handler
            CmiCallHandler(handler, msg);
        }

        CcdCallBacks();

        // TODO: suspend? or spin?
    }
}

// TODO: implement CsdEnqueue/Dequeue (why are these necessary?)
