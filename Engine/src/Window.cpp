#include "Window.hpp"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

Window::Window(uint32_t width, uint32_t height, std::string_view const &title)
    : m_width(width), m_height(height) {
  glfwInit();
  m_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  glfwMakeContextCurrent(m_handle);

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Window::new_frame() {
  glfwPollEvents();
  glfwSwapBuffers(m_handle);
}

bool Window::should_close() const { return glfwWindowShouldClose(m_handle); }

void Window::size(uint32_t width, uint32_t height) {
  glfwSetWindowSize(m_handle, width, height);
  m_width = width;
  m_height = height;
}

uint32_t Window::width() const { return m_width; }
uint32_t Window::height() const { return m_height; }
