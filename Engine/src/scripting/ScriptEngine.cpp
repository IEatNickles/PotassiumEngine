#include "scripting/ScriptEngine.hpp"
#include "utils/PlatformUtils.hpp"

#include "Log.hpp"

#define REGISTER_FUNCTION(name)                                                \
  m_##name##_fn =                                                              \
      (decltype(m_##name##_fn))PlatformUtils::load_dl_sym(handle, #name);      \
  DEBUG_ASSERT(m_##name##_fn, PlatformUtils::dl_error());

namespace KEngine {
void ScriptEngine::init(std::filesystem::path const &script_dl) {
  void *handle = PlatformUtils::open_dl(script_dl);
  DEBUG_ASSERT(handle, PlatformUtils::dl_error());

  REGISTER_FUNCTION(add_component);
  REGISTER_FUNCTION(draw_components);
  REGISTER_FUNCTION(draw_add_component);
  REGISTER_FUNCTION(save_components);
  REGISTER_FUNCTION(load_components);
}
} // namespace KEngine

#undef REGISTER_FUNCTION
