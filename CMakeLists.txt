
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 20)
project(LibRtRender VERSION 0.1.0 LANGUAGES CXX)

find_package(Vulkan REQUIRED)

# Create library target
add_library(rtintersect STATIC
    lib/api.hpp
    lib/device_factory.hpp
    lib/device_factory.cpp
    lib/device.hpp
    lib/index_buffer.hpp
    lib/index_buffer.cpp
    lib/vertex_buffer.hpp
    lib/vertex_buffer.cpp
    lib/frame_buffer.hpp
    lib/frame_buffer.cpp
    lib/pixel_format.hpp
    lib/result.hpp
    lib/float3.hpp
    lib/float3.cpp
    lib/bounding_box.hpp
    lib/bounding_box.cpp
    lib/bottom_level_acceleration_structure.hpp
    lib/bottom_level_acceleration_structure.cpp
    lib/intersector.hpp
    lib/intersector.cpp
    lib/command_buffer.hpp
    lib/command_buffer.cpp

    lib/cpu/device.hpp
    lib/cpu/device.cpp
    lib/cpu/index_buffer.hpp
    lib/cpu/index_buffer.cpp
    lib/cpu/vertex_buffer.hpp
    lib/cpu/vertex_buffer.cpp
    lib/cpu/frame_buffer.hpp
    lib/cpu/frame_buffer.cpp
    lib/cpu/bottom_level_acceleration_structure.hpp
    lib/cpu/bottom_level_acceleration_structure.cpp

    lib/vulkan/device.hpp
    lib/vulkan/device.cpp
    lib/vulkan/index_buffer.hpp
    lib/vulkan/index_buffer.cpp
    lib/vulkan/vertex_buffer.hpp
    lib/vulkan/vertex_buffer.cpp
    lib/vulkan/frame_buffer.hpp
    lib/vulkan/frame_buffer.cpp
)

set_target_properties(rtintersect PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(rtintersect Vulkan::Vulkan)

# Create executable target
add_executable(RtTrace
    src/main.cpp
)

# Link executable target with library target
target_include_directories(RtTrace PRIVATE lib)
target_link_libraries(RtTrace rtintersect)
