# pragma once

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
public:
	ThreadPool() : ThreadPool(10) {}
	explicit ThreadPool(uint32_t nbThreads);
	~ThreadPool();

	void Enqueue(std::function<void()> const& func);
	void WaitUntilFinished();

private:
	void CheckQueue();

	std::vector<std::thread> m_workers;
	std::queue<std::function<void()>> m_taskQueue;
	std::mutex m_queueMutex;
	std::condition_variable m_waitCondition;
	std::condition_variable m_finishCondition;
	std::atomic<bool> m_stop{ false };
	std::atomic<int> m_tasksRemaining{ 0 };
	uint32_t m_nbThreads = 0;
};