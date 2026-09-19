#pragma once

#include "export/ExportEngine/ExportJob.h"
#include "core/Tasks/TaskScheduler.h"
#include <vector>
#include <memory>
#include <mutex>

namespace jaguar {

class ExportQueue {
public:
    static ExportQueue& Instance();

    std::string AddJob(const Project& project, const EncoderConfig& config);
    void CancelJob(const std::string& jobId);
    void PauseJob(const std::string& jobId);
    void ResumeJob(const std::string& jobId);

    std::vector<std::shared_ptr<ExportJob>> GetAllJobs() const;

private:
    ExportQueue() = default;

    void ProcessQueue();

    mutable std::mutex m_mutex;
    std::vector<std::shared_ptr<ExportJob>> m_jobs;
    bool m_isProcessing{ false };
};

} // namespace jaguar
