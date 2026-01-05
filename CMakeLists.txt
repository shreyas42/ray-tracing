cmake_minimum_required(VERSION 4.0)
project(RayTracingBasics)

file(GLOB sources "src/*.cpp")
set(includedir "include/")

add_executable(Main ${sources})
target_include_directories(Main PRIVATE ${includedir})
