#pragma once

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

typedef std::function<void()> fn_type;

class Worker
{
public:

    Worker()
        :enabled(true),fqueue()
        ,thread(&Worker::thread_fn, this)
    {}
    ~Worker()
    {
        enabled = false;
        cv.notify_one();
        thread.join();
    }
    void appendFn(fn_type fn)
    {
        std::unique_lock<std::mutex> locker(mutex);
        fqueue.push(fn);
        cv.notify_one();
    }
    size_t getTaskCount()
    {
        std::unique_lock<std::mutex> locker(mutex);
        return fqueue.size();
    }
    bool   isEmpty()
    {
        std::unique_lock<std::mutex> locker(mutex);
        return fqueue.empty();
    }

private:

    bool				enabled;
    std::condition_variable		cv;
    std::queue<fn_type>			fqueue;
    std::mutex				mutex;
    std::thread				thread;

    void thread_fn()
    {
        while (enabled)
        {
            std::unique_lock<std::mutex> locker(mutex);
            // Ожидаем уведомления, и убедимся что это не ложное пробуждение
            // Поток должен проснуться если очередь не пустая либо он выключен
            cv.wait(locker, [&](){ return !fqueue.empty() || !enabled; });
            while(!fqueue.empty())
            {
                fn_type fn = fqueue.front();
                // Разблокируем мютекс перед вызовом функтора
                locker.unlock();
                fn();
                // Возвращаем блокировку снова перед вызовом fqueue.empty()
                locker.lock();
                fqueue.pop();
            }
        }
    }
};
