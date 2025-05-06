#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t num_threads)
{
    // Creating worker threads
    for (size_t i = 0; i < num_threads; i++)
    {
        this->threads.emplace_back([this]
        {
            while (true)
            {
                std::function<void()> task;
                {
                    // Locking the queue so that data can be shared safely
                    std::unique_lock<std::mutex> lock(this->queue_mutex);

                    // Waiting until there is a task to execute or the pool is stopped
                    this->cv.wait(lock, [this] {
                        return !this->tasks.empty() || this->stop;
                    });

                    // exit the thread in case the pool is stopped and there are no tasks
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }
                    
                    // Get the next task from the queue
                    task = std::move(this->tasks.front());
                    this->tasks.pop();

                }
                task();
            } 
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        // Lock the queue to update the stop flag safely
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        this->stop = true;
    }

    // Notify all threads
    this->cv.notify_all();

    // Joining all worker threads to ensure they have completed their tasks
    for (auto& thread : this->threads) thread.join();
}