#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
class MutexAccessControl {
public:
    static std::mutex mutex;
    
    static void acquire() {
        mutex.lock();
    }
    
    static void release() {
        mutex.unlock();
    }
};

std::mutex MutexAccessControl::mutex;



// An MPSC queue that can be used to send messages between threads.
template<typename ConcreteQ, typename MessageType, typename AccessControlPolicy>
class MPSCQueue {
    ConcreteQ q;

public:
    MessageType pop() {
        AccessControlPolicy::acquire();
        // This will not work for atomics.
        // It's fine for now: internal implementation detail.
        MessageType message = q.front();
        q.pop();
        AccessControlPolicy::release();
        return message;
    }

    void push(MessageType message) {
        AccessControlPolicy::acquire();
        q.push(message);
        AccessControlPolicy::release();
    }

};

template<typename MessageType>
using ReconverseQueue = MPSCQueue<std::queue<MessageType>, MessageType, MutexAccessControl>;

#endif
