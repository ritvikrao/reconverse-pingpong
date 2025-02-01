#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

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
        {              // Last thread to arrive
            count = 0; // Reset barrier for potential reuse
            cv.notify_all();
        }
        else
        {
            cv.wait(lock, [this]
                    { return count == 0; });
        }
    }
};