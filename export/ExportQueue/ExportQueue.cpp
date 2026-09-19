#include "ExportQueue.h"
#include "core/Logging/Logger.h"

namespace jaguar {

ExportQueue& ExportQueue::Instance() {
    static ExportQueue instance;
    return instance;
}

std::string ExportQueue::AddJob(const Project& project, const EncoderConfig& config) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string jobId = "job_" + std::to_string(m_jobs.size() + 1);
    auto job = std::make_shared<ExportJob>(jobId, project, config);
    m_jobs.push_back(job);

    LOG_INFO("ExportQueue", "Added job to queue: " + jobId);
    ProcessQueue();
    return jobId;
}

void ExportQueue::CancelJob(const std::string& jobId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& job : m_jobs) {
        if (job->GetId() == jobId) {
            job->Cancel();
            break;
        }
    }
}

void ExportQueue::PauseJob(const std::string& jobId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& job : m_jobs) {
        if (job->GetId() == jobId) {
            job->Pause();
            break;
        }
    }
}

void ExportQueue::ResumeJob(const std::string& jobId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& job : m_jobs) {
        if (job->GetId() == jobId) {
            job->Resume();
            break;
        }
    }
}

std::vector<std::shared_ptr<ExportJob>> ExportQueue::GetAllJobs() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_jobs;
}

void ExportQueue::ProcessQueue() {
    if (m_isProcessing) return;

    for (auto& job : m_jobs) {
        if (job->GetState() == ExportState::Waiting) {
            m_isProcessing = true;
            TaskScheduler::Instance().Enqueue([this, job]() {
                job->Start();
                {
                    std::lock_guard<std::mutex> lock(this->m_mutex);
                    this->m_isProcessing = false;
                }
                this->ProcessQueue();
            });
            break;
        }
    }
}

} // namespace jaguar
