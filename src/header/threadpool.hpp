#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); // 完美转发
}

namespace vks
{
    class Thread
    {
    private:
        bool destroying = false;
        std::thread worker;
        std::queue<std::function<void()>> jobQueue;
        std::mutex queueMutex;
        std::condition_variable condition;
        void queueLoop()
        {
            while (true)
            {
                std::function<void()> job;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this]
                                   { return !jobQueue.empty() || destroying; });
                    if (destroying)
                        break;
                    job = jobQueue.front();
                }

                job();

                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    jobQueue.pop();
                    condition.notify_one();
                }
            }
        }

    public:
        Thread()
        {
            worker = std::thread(&Thread::queueLoop, this);
        }
        ~Thread()
        {
            if (worker.joinable())
            {
                wait();
                queueMutex.lock();
                destroying = true;
                condition.notify_one();
                queueMutex.unlock();
                worker.join();
            }
        }
        void addJob(std::function<void()> function)
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            jobQueue.push(std::move(function));
            condition.notify_all();
        }

        void wait()
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]()
                           { return jobQueue.empty(); });
        }
    };

    class ThreadPool
    {
    public:
        std::vector<std::unique_ptr<Thread> >threads;

        void setThreadCount(uint32_t count) {
            threads.clear();
            for(auto i=0;i<count;++i) {
                threads.push_back(make_unique<Thread>());
            }
        }

        void wait() {
            for(auto &thread:threads) {
                thread->wait();
            }
        }
        ThreadPool(){

        }
        ~ThreadPool(){

        }
    };
}
