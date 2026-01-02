#include "ThreadPool.h"

ThreadPool::ThreadPool(const uint32_t nbThreads) : m_nbThreads(nbThreads)
{
    for (uint32_t i = 0; i < m_nbThreads; i++)
        m_workers.emplace_back(&ThreadPool::CheckQueue, this);
}

ThreadPool::~ThreadPool()
{
    m_stop.store(true, std::memory_order_release);
    m_waitCondition.notify_all();

    for (auto& worker : m_workers)
        if (worker.joinable())
            worker.join();

    m_workers.clear();
}

void ThreadPool::Enqueue(std::function<void()> const& func)
{
    m_tasksRemaining.fetch_add(1, std::memory_order_acq_rel);
    {
        std::unique_lock lock(m_queueMutex);
        m_taskQueue.emplace(func);
    }
    m_waitCondition.notify_one();
}

void ThreadPool::WaitUntilFinished()
{
    std::unique_lock lock(m_queueMutex);
    m_finishCondition.wait(lock, [this]() { return m_tasksRemaining.load(std::memory_order_acquire) == 0 && m_taskQueue.empty(); });
}

void ThreadPool::CheckQueue()
{
    while (true)
    {
        std::function<void()> task;

        {
            std::unique_lock lock(m_queueMutex);
            m_waitCondition.wait(lock, [this]() { return m_stop.load() || !m_taskQueue.empty(); });

            if (m_stop.load() && m_taskQueue.empty())
                return;

            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
        }

        task();
        m_tasksRemaining.fetch_sub(1, std::memory_order_acq_rel);
        m_finishCondition.notify_one();
    }
}