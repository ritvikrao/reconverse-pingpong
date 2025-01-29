#include "scheduler.h"
#include "convcore.h"
#include "queue.h"

void CsdScheduler()
{
    // get pthread level queue
    ConverseQueue<CmiMessage> queue = CmiGetState().queue;

    while (true)
    {
        // get next event
        // TODO: where is the check for empty queue?
        CmiMessage message = queue.pop();
        // process event
    }
}
