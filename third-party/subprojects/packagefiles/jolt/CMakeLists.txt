# Jolt keeps its CMakeLists.txt in Build/; Meson's cmake module needs one at the source root.
# Being a non-root project also makes Jolt skip its samples, tests and viewer targets.
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(JoltPhysicsWrap LANGUAGES CXX)

add_subdirectory(Build)
