#pragma once

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <atomic>
#include <memory>

namespace jaguar {

class TaskScheduler {
public:
    static TaskScheduler& Instance();

    void Initialize(size_t threadCount = 4);
    void Shutdown();

    template<class F, class... Args>
    void Enqueue(F&& f, Args&&... args) {
        auto task = std::make_shared<std::function<void()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            if (m_stop) return;
            m_tasks.emplace([task]() { (*task)(); });
        }
        m_cv.notify_one();
    }

private:
    TaskScheduler() = default;
    ~TaskScheduler();

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;

    std::mutex m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop{ false };
};

} // namespace jaguar
