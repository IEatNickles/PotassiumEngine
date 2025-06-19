#pragma once

#include <cstdint>

namespace KEngine {
struct FrameBufferInfo {
  uint8_t color_attribute_count;
  bool depth_buffer;
  bool stencil_buffer;
};

class FrameBuffer {
public:
  FrameBuffer();
  FrameBuffer(FrameBufferInfo const &info);

private:
  uint32_t m_fbo;
};
} // namespace KEngine
