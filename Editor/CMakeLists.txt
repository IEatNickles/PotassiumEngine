cmake_minimum_required(VERSION 3.8)
project(potassium-editor LANGUAGES C CXX)

set(SRC
  src/editor_app.cpp
  src/scripting/CodeGen.cpp
)
add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} potassium)

target_include_directories(${PROJECT_NAME} PUBLIC
  include/

  ../Engine/vendor/tinyfiledialogs/

  PRIVATE

  ../Engine/vendor/imgui/
  ../Engine/vendor/glfw/include/
  ../Engine/vendor/glad/include/
  ../Engine/include/
)
