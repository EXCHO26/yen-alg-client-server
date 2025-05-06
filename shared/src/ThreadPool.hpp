#ifndef THREAD_POOL_HEADER
#define THREAD_POOL_HEADER

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <future>

class ThreadPool
{
    public:
        ThreadPool(size_t num_threads = std::thread::hardware_concurrency());
        ~ThreadPool();

        // Enqueue task for execution by the thread pool
        template<class F, class... Args>
        void enqueue(F&& f, Args&&... args) 
        {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                if(stop) throw std::runtime_error("enqueue on stopped ThreadPool");
                tasks.emplace([=]{ f(args...); });
            }
            cv.notify_one();
        }

    private:
        // Vector to store worker threads
        std::vector<std::thread> threads;

        // Queue of tasks
        std::queue<std::function<void()> > tasks;

        // Mutex to synchronize access to shared data
        std::mutex queue_mutex;

        // Condition variable to signal changes in the state of the tasks queue
        std::condition_variable cv;

        // Flag to indicate whether the thread pool should stop or not
        bool stop = false;
};

#endif