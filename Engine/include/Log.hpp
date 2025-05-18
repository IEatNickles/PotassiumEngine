#pragma once

#include <cassert>
#include <cstdlib>
#include <format>
#include <iostream>
#include <ostream>
#include <signal.h>
#include <string>
#include <string_view>

namespace KEngine {
namespace Severity {
enum Type {
  TRACE,
  WARN,
  ERROR,
  FATAL,
};

inline std::string_view color(Type s) {
  assert(s <= FATAL);

  switch (s) {
  case TRACE:
    return "\033[1m";
  case WARN:
    return "\033[1;33m";
  case ERROR:
    return "\033[1;31m";
  case FATAL:
    return "\033[1;41m";
  }
  return "";
}

inline std::ostream &operator<<(std::ostream &out, Type s) {
  assert(s <= FATAL);

  out << color(s);
  switch (s) {
  case TRACE:
    out << "TRACE";
    break;
  case WARN:
    out << "WARN";
    break;
  case ERROR:
    out << "ERROR";
    break;
  case FATAL:
    out << "FATAL";
    break;
  }

  return out << "\033[0m";
}
}; // namespace Severity

class Logger {
public:
  Logger(const std::string &name) : m_name(name) {}

  template <typename... Ts>
  inline void trace(int line, char const *file, std::string_view fmt,
                    Ts &&...args) {
    log(line, file, Severity::TRACE, fmt, args...);
  }

  template <typename... Ts>
  inline void warn(size_t line, char const *file, std::string_view fmt,
                   Ts &&...args) {
    log(line, file, Severity::WARN, fmt, args...);
  }

  template <typename... Ts>
  inline void error(size_t line, char const *file, std::string_view fmt,
                    Ts &&...args) {
    log(line, file, Severity::ERROR, fmt, args...);
  }

  template <typename... Ts>
  inline void fatal(size_t line, char const *file, std::string_view fmt,
                    Ts &&...args) {
    log(line, file, Severity::FATAL, fmt, args...);
    raise(SIGTRAP);
  }

private:
  template <typename... Ts>
  inline void log(size_t line, char const *file, Severity::Type severity,
                  std::string_view fmt, Ts &&...args) {
    std::string msg = std::vformat(fmt, std::make_format_args(args...));
    std::cout << m_name << " [" << severity << "] " << msg << " (" << file
              << "):" << line << "\n";
  }

private:
  std::string m_name;
};
} // namespace KEngine

#ifdef ENGINE_API_EXPORT
inline static KEngine::Logger logger("CORE");
#else
inline static KEngine::Logger logger("CLIENT");
#endif

#define DEBUG_TRACE(...) logger.trace(__LINE__, __FILE__, __VA_ARGS__);
#define DEBUG_WARN(...) logger.warn(__LINE__, __FILE__, __VA_ARGS__);
#define DEBUG_ERROR(...) logger.error(__LINE__, __FILE__, __VA_ARGS__);
#define DEBUG_FATAL(...) logger.fatal(__LINE__, __FILE__, __VA_ARGS__);
#define DEBUG_ASSERT(condition, ...)                                           \
  if (!(condition))                                                            \
    logger.fatal(__LINE__, __FILE__, __VA_ARGS__);
