#pragma once

#include <cstdint>
#include <string_view>

class Window final {
public:
  Window(uint32_t width, uint32_t height, std::string_view const &title);

  bool should_close() const;

  void size(uint32_t width, uint32_t height);

  void new_frame();

  uint32_t width() const;
  uint32_t height() const;

private:
  uint32_t m_width, m_height;
  struct GLFWwindow *m_handle;
};
