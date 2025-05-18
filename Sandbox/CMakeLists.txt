cmake_minimum_required(VERSION 3.8)
project(sandbox LANGUAGES C CXX)

set(SRC
  src/SandboxApp.cpp
)
add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} potassium)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/Sandbox/src/
)
