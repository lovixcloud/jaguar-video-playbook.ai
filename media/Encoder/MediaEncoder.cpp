#include "MediaEncoder.h"
#include "core/Logging/Logger.h"
#include <filesystem>

namespace jaguar {

bool MediaEncoder::Initialize(const EncoderConfig& config) {
    m_config = config;
    std::filesystem::path p(config.outputPath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }

    m_outputFile.open(config.outputPath, std::ios::out | std::ios::binary);
    if (!m_outputFile.is_open()) {
        LOG_ERROR("MediaEncoder", "Failed to open output file: " + config.outputPath);
        return false;
    }

    m_isInitialized = true;
    m_encodedFrames = 0;
    LOG_INFO("MediaEncoder", "Initialized encoder for file: " + config.outputPath);
    return true;
}

bool MediaEncoder::EncodeFrame(const MediaFrame& frame) {
    if (!m_isInitialized || !m_outputFile.is_open()) return false;

    if (frame.type == MediaFrame::Type::Video) {
        m_encodedFrames++;
    }
    return true;
}

bool MediaEncoder::Finalize() {
    if (!m_isInitialized) return false;

    if (m_outputFile.is_open()) {
        const char header[] = "JAGUAR_MP4_HEADER_V1";
        m_outputFile.write(header, sizeof(header));
        m_outputFile.close();
    }

    m_isInitialized = false;
    LOG_INFO("MediaEncoder", "Finalized encoded file.");
    return true;
}

} // namespace jaguar
