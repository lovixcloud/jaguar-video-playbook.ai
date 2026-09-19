#include "IVideoEffect.h"
#include <algorithm>
#include <cmath>

namespace jaguar {

void ColorAdjustEffect::Apply(MediaFrame& frame, const EffectParameters& params) {
    if (frame.pixelData.empty() || frame.type != MediaFrame::Type::Video) return;

    uint8_t* pixels = frame.pixelData.data();
    size_t count = frame.pixelData.size() / 4;

    double brightnessOffset = params.brightness * 255.0;
    double contrastFactor = params.contrast;

    for (size_t i = 0; i < count; ++i) {
        size_t idx = i * 4;
        for (int c = 0; c < 3; ++c) {
            double val = static_cast<double>(pixels[idx + c]);
            val = (val - 128.0) * contrastFactor + 128.0 + brightnessOffset;
            pixels[idx + c] = static_cast<uint8_t>(std::clamp(val, 0.0, 255.0));
        }
    }
}

void GrayscaleEffect::Apply(MediaFrame& frame, const EffectParameters& params) {
    if (!params.grayscale || frame.pixelData.empty() || frame.type != MediaFrame::Type::Video) return;

    uint8_t* pixels = frame.pixelData.data();
    size_t count = frame.pixelData.size() / 4;

    for (size_t i = 0; i < count; ++i) {
        size_t idx = i * 4;
        uint8_t b = pixels[idx];
        uint8_t g = pixels[idx + 1];
        uint8_t r = pixels[idx + 2];
        uint8_t gray = static_cast<uint8_t>(0.114 * b + 0.587 * g + 0.299 * r);
        pixels[idx] = gray;
        pixels[idx + 1] = gray;
        pixels[idx + 2] = gray;
    }
}

void VignetteEffect::Apply(MediaFrame& frame, const EffectParameters& params) {
    if (params.vignette <= 0.0 || frame.pixelData.empty() || frame.type != MediaFrame::Type::Video) return;

    uint8_t* pixels = frame.pixelData.data();
    int w = frame.width;
    int h = frame.height;
    double cx = w / 2.0;
    double cy = h / 2.0;
    double maxDist = std::sqrt(cx * cx + cy * cy);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            size_t idx = (y * w + x) * 4;
            double dx = x - cx;
            double dy = y - cy;
            double dist = std::sqrt(dx * dx + dy * dy) / maxDist;
            double factor = 1.0 - (dist * params.vignette);
            factor = std::clamp(factor, 0.0, 1.0);

            pixels[idx] = static_cast<uint8_t>(pixels[idx] * factor);
            pixels[idx + 1] = static_cast<uint8_t>(pixels[idx + 1] * factor);
            pixels[idx + 2] = static_cast<uint8_t>(pixels[idx + 2] * factor);
        }
    }
}

} // namespace jaguar
