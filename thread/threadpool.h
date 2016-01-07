#pragma once

#include "threadworker.h"
#include <windows.h>

typedef std::shared_ptr<Worker> worker_ptr;

class ThreadPool
{
public:

    ThreadPool()
    {
        size_t threads = std::thread::hardware_concurrency();
        if (threads == 0)
        {
            threads = 1;
        }
        for (size_t i = 0; i < threads; i++)
        {
            worker_ptr pWorker(new Worker);
            m_workers.push_back(pWorker);
        }
    }
    ~ThreadPool() {}

    template<class _FN, class... _ARGS>
    void runAsync(_FN _fn, _ARGS... _args)
    {
        getFreeWorker()->appendFn(std::bind(_fn,_args...));
    }

    void waitForFree()
    {
        bool isEmpty = false;
        while (!isEmpty)
        {
            isEmpty = true;
            for (auto &it : m_workers)
            {
                if (!it->isEmpty())
                {
                    isEmpty = false;
                    break;
                }
            }
            Sleep(1);
        }
    }

private:

    worker_ptr getFreeWorker()
    {
        worker_ptr pWorker;
        size_t minTasks = UINT32_MAX;
        for (auto &it : m_workers)
        {
            if (it->isEmpty())
            {
                return it;
            }
            else if (minTasks > it->getTaskCount())
            {
                minTasks = it->getTaskCount();
                pWorker = it;
            }
        }
        return pWorker;
    }

    std::vector<worker_ptr> m_workers;
};

