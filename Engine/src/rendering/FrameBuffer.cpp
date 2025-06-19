#include "rendering/FrameBuffer.hpp"

#include "glad/glad.h"

namespace KEngine {
FrameBuffer::FrameBuffer(FrameBufferInfo const &info) {
  glGenFramebuffers(1, &m_fbo);
}
} // namespace KEngine
