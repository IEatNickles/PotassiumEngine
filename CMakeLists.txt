cmake_minimum_required(VERSION 3.8)
project(engine-all LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(K_VERSION_MINOR=0 K_VERSION_MAJOR=0)

add_subdirectory(Engine)
add_subdirectory(Sandbox)
add_subdirectory(Editor)
