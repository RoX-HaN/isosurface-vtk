cmake_minimum_required(VERSION 3.10)
project(IsosurfaceVTK)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(iso iso_vir.cpp)

target_link_libraries(iso PRIVATE ${VTK_LIBRARIES})
