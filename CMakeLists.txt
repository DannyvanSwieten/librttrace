
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 20)
project(LibRtTrace VERSION 0.1.0 LANGUAGES CXX)

# Create library target
add_library(rttrace STATIC
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
    lib/bounding_box.hpp
    lib/bounding_box.cpp

    lib/cpu/device.hpp
    lib/cpu/device.cpp
    lib/cpu/index_buffer.hpp
    lib/cpu/index_buffer.cpp
    lib/cpu/vertex_buffer.hpp
    lib/cpu/vertex_buffer.cpp
    lib/cpu/frame_buffer.hpp
    lib/cpu/frame_buffer.cpp
    lib/cpu/acceleration_structure.hpp
    lib/cpu/acceleration_structure.cpp
)

set_target_properties(rttrace PROPERTIES LINKER_LANGUAGE CXX)

# Create executable target
add_executable(RtTrace
    src/main.cpp
)

# Link executable target with library target
target_include_directories(RtTrace PRIVATE lib)
target_link_libraries(RtTrace rttrace)
