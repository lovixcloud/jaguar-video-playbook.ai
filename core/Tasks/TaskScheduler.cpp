#include "TaskScheduler.h"

namespace jaguar {

TaskScheduler& TaskScheduler::Instance() {
    static TaskScheduler instance;
    return instance;
}

TaskScheduler::~TaskScheduler() {
    Shutdown();
}

void TaskScheduler::Initialize(size_t threadCount) {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    if (!m_workers.empty()) return;
    m_stop = false;

    for (size_t i = 0; i < threadCount; ++i) {
        m_workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->m_queueMutex);
                    this->m_cv.wait(lock, [this]() {
                        return this->m_stop || !this->m_tasks.empty();
                    });
                    if (this->m_stop && this->m_tasks.empty()) return;
                    task = std::move(this->m_tasks.front());
                    this->m_tasks.pop();
                }
                task();
            }
        });
    }
}

void TaskScheduler::Shutdown() {
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
    m_cv.notify_all();
    for (std::thread& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    m_workers.clear();
}

} // namespace jaguar
