#ifndef WORKER_H
#define WORKER_H
#include <iostream>
#include <queue>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <condition_variable>

class Worker
{
public:
    explicit Worker(const unsigned int pool_size);
    ~Worker();
    void submit(std::function<void()> func);
    bool isDone();
    void stop();

private:
    void run();

private:
    std::vector<std::thread> _threads;
    std::shared_timed_mutex _mutex;
    std::mutex waitQueue;
    std::atomic_bool stop_;
    std::atomic_int _threadsLen;
    std::condition_variable_any _cv;
    std::queue<std::function<void()> > work_queue_;
};
#endif // WORKER_H
