cmake_minimum_required(VERSION 3.8)
project(potassium LANGUAGES C CXX)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/yaml-cpp)
add_subdirectory(vendor/json)
add_subdirectory(vendor/flecs)

add_compile_definitions(K_API_EXPORT)
if(WIN32)
  add_compile_definitions(K_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
  add_compile_definitions(K_PLATFORM_LINUX)
else()
  add_compile_definitions(K_PLATFORM_APPLE)
endif()

set(SRC
  src/main.cpp
  src/Window.cpp
  src/Scene.cpp
  src/Renderer.cpp
  src/Shader.cpp
  src/scripting/ScriptEngine.cpp

  src/utils/LinuxUtils.cpp

  src/UUID.cpp

  # glad
  vendor/glad/src/glad.c

  # imgui
  vendor/imgui/imgui.cpp
  vendor/imgui/imgui_demo.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_tables.cpp
  vendor/imgui/imgui_widgets.cpp
  vendor/imgui/misc/cpp/imgui_stdlib.cpp

  vendor/imgui/backends/imgui_impl_opengl3.cpp
  ./vendor/imgui/backends/imgui_impl_glfw.cpp

  # tinyfiledialogs
  vendor/tinyfiledialogs/tinyfiledialogs.c
)
add_library(${PROJECT_NAME} SHARED ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC glfw yaml-cpp nlohmann_json flecs)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_SOURCE_DIR}/Engine/include/

  ${CMAKE_SOURCE_DIR}/Engine/vendor/glm/
  ${CMAKE_SOURCE_DIR}/Engine/vendor/entt/include/
  ${CMAKE_SOURCE_DIR}/Engine/vendor/msgpack/
  ${CMAKE_SOURCE_DIR}/Engine/vendor/imgui/
  ${CMAKE_SOURCE_DIR}/Engine/vendor/json/include/
  ${CMAKE_SOURCE_DIR}/Engine/vendor/flecs/include/

  PRIVATE
  ${CMAKE_SOURCE_DIR}/Engine/vendor/glfw/include/
  ${CMAKE_SOURCE_DIR}/Engine/vendor/glad/include/
)
