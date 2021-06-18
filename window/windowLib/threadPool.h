#pragma once
#include <thread>
#include <array>
#include <chrono>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

template<size_t S>
class ThreadPool
{
public:
    ThreadPool()
    {
        for (size_t i{ 0 }; i < S; ++i)
        {
            mWorkers[i] = std::thread(&ThreadPool::workerProc, this);
        }
    }

    ~ThreadPool()
    {
        mStopping = true;
        mCv.notify_all();
        for (size_t i{ 0 }; i < S; ++i)
        {
            mWorkers[i].join();
        }
    }

    using Job = std::function<void()>;
    using Lock = std::lock_guard<std::mutex>;

    // Post a job
    void post(Job job)
    {
        {
            Lock lock(mJobsMutex);
            mPendingJobs.push(std::move(job));
        }
        mCv.notify_one();
    }

private:
    // worker threads
    std::array<std::thread, S> mWorkers;

    bool mStopping{ false };
    std::queue<Job> mPendingJobs;
    std::mutex mJobsMutex;
    std::condition_variable mCv;

    void workerProc()
    {
        while (true)
        {
            Job job;
            {
                std::unique_lock<std::mutex> lock(mJobsMutex);
                mCv.wait(lock, [this]() { return mStopping || !mPendingJobs.empty(); });
                if (mStopping && mPendingJobs.empty())
                    break;
                job = std::move(mPendingJobs.front());
                mPendingJobs.pop();
            }
            job();
        }
        //std::this_thread::sleep_for(std::chrono::seconds(15));
    }
};