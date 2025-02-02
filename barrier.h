#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// TODO: this is not reusable - on trying to reuse barrier it hangs
class Barrier
{
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    int thread_count;

public:
    explicit Barrier(int num_threads) : count(0), thread_count(num_threads) {}

    void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count++;

        if (count == thread_count)
        {
            count = 0; // Reset barrier for potential reuse
            cv.notify_all();
            lock.unlock();
        }
        else
        {
            cv.wait(lock, [this]
                    { return count == 0; });
        }
    }
};