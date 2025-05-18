cmake_minimum_required(VERSION 3.8)
project(scripts)

set(CMAKE_CXX_STANDARD 20)

add_library(scripts SHARED ./generated/init.cpp)
link_directories(
  ../../../build/Engine/vendor/yaml-cpp/
  ../../../build/Engine/
)
target_link_libraries(scripts -L../../../build/Engine/vendor/yaml-cpp/yaml-cppd -L../../../build/Engine/potassium)

target_include_directories(scripts PRIVATE
  ./
  ../../../Engine/include/
  ../../../Engine/vendor/imgui/
  ../../../Engine/vendor/yaml-cpp/include/
  ../../../Engine/vendor/entt/include/
)
