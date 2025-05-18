#include "scripting/ScriptEngine.hpp"
#include "utils/PlatformUtils.hpp"

#include "Log.hpp"

#define REGISTER_FUNCTION(name, type)                                          \
  m_##name##_fn = (type)PlatformUtils::load_dl_sym(handle, #name);             \
  DEBUG_ASSERT(m_##name##_fn, PlatformUtils::dl_error());

namespace KEngine {
void ScriptEngine::init(std::filesystem::path const &script_dl) {
  void *handle = PlatformUtils::open_dl(script_dl);
  DEBUG_ASSERT(handle, PlatformUtils::dl_error());

  REGISTER_FUNCTION(add_component, AddComponentFn);
  REGISTER_FUNCTION(draw_components, DrawComponentsFn);
  REGISTER_FUNCTION(draw_add_component, DrawAddComponentFn);
  REGISTER_FUNCTION(save_components, SaveComponentsFn);
  REGISTER_FUNCTION(load_components, LoadComponentsFn);
}
} // namespace KEngine

#undef REGISTER_FUNCTION
