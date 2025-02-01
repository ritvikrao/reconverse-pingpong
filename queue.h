#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>

class MutexAccessControl
{
public:
    static std::mutex mutex;
    static void acquire();
    static void release();
};

// An MPSC queue that can be used to send messages between threads.
template <typename ConcreteQ, typename MessageType, typename AccessControlPolicy>
class MPSCQueue
{
    ConcreteQ q;

public:
    MessageType pop()
    {
        AccessControlPolicy::acquire();
        // This will not work for atomics.
        // It's fine for now: internal implementation detail.

        if (q.size() == 0)
        {
            // TODO: throw something?
            throw std::runtime_error("Cannot pop from empty queue is empty");
        }

        MessageType message = q.front();
        q.pop();
        AccessControlPolicy::release();
        return message;
    }

    void push(MessageType message)
    {
        AccessControlPolicy::acquire();
        q.push(message);
        AccessControlPolicy::release();
    }

    bool empty()
    {

        return this->size() == 0;
    }

    size_t size()
    {
        AccessControlPolicy::acquire();
        size_t result = q.size();
        AccessControlPolicy::release();

        return result;
    }
};

template <typename MessageType>
using ConverseQueue = MPSCQueue<std::queue<MessageType>, MessageType, MutexAccessControl>;

#endif
