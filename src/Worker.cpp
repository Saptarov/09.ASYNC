#include "Worker.h"

Worker::Worker(const unsigned int pool_size)
    : stop_(false)
    , _threadsLen(0)
{
    for(uint i = 0; i < pool_size; ++i) {
        _threads.emplace_back(std::thread(std::bind(&Worker::run, this)));
    }
}

Worker::~Worker() {
    stop_ = true;
    for(auto& thread : _threads) {
        thread.join();
    }
}

void Worker::submit(std::function<void()> func) {
    std::lock_guard<std::mutex> guard(waitQueue);
    work_queue_.push(func);
    _cv.notify_all();
}

bool Worker::isDone() {
    std::lock_guard<std::mutex> guard(this->waitQueue);
    bool workersIsDone = _threadsLen == 0 && work_queue_.empty();
    return workersIsDone;
}

void Worker::stop() {
    stop_ = true;
}

void Worker::run()
{
    std::function<void()> func;
    while(!this->stop_)
    {
        std::shared_lock<std::shared_timed_mutex> lock(this->_mutex, std::try_to_lock);
        this->_cv.wait_for(lock, std::chrono::milliseconds(10));
        std::lock_guard<std::mutex> guard(this->waitQueue);
        if(!this->work_queue_.empty())
        {
            func = this->work_queue_.front();
            this->work_queue_.pop();
            func();
        }
    }
}
