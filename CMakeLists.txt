
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 20)
project(LibRtRender VERSION 0.1.0 LANGUAGES CXX)

find_package(Vulkan REQUIRED)

# Create library target
add_library(rtintersect STATIC
    lib/api.hpp
    lib/overloaded.hpp
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
    lib/top_level_acceleration_structure.hpp
    lib/top_level_acceleration_structure.cpp
    lib/intersector.hpp
    lib/intersector.cpp
    lib/command_buffer.hpp
    lib/command_buffer.cpp
    lib/resource_ctx.hpp
    lib/resource_ctx.cpp
    lib/image.hpp
    lib/image.cpp
    lib/pipeline.hpp
    lib/pipeline.cpp
    lib/ray_generation_program.hpp
    lib/ray_generation_program.cpp
    lib/closest_hit_program.hpp
    lib/closest_hit_program.cpp
    lib/shader_compiler.hpp
    lib/shader_compiler.cpp
    lib/miss_program.hpp
    lib/miss_program.cpp

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
    lib/cpu/top_level_acceleration_structure.hpp
    lib/cpu/top_level_acceleration_structure.cpp
    lib/cpu/command_buffer.hpp
    lib/cpu/command_buffer.cpp
    lib/cpu/intersect.hpp
    lib/cpu/intersect.cpp
    lib/cpu/ray_generation_program.hpp
    lib/cpu/ray_generation_program.cpp
    lib/cpu/closest_hit_program.hpp
    lib/cpu/closest_hit_program.cpp
    lib/cpu/miss_program.hpp
    lib/cpu/miss_program.cpp

    lib/cpu/virtual_machine/vm.hpp
    lib/cpu/virtual_machine/vm.cpp
    lib/cpu/virtual_machine/shader_program.hpp

    lib/gpu/ray_generation_program.hpp
    lib/gpu/ray_generation_program.cpp

    lib/shader_graph/shader_graph.hpp
    lib/shader_graph/shader_graph.cpp
    lib/shader_graph/context.hpp
    lib/shader_graph/context.cpp
    lib/shader_graph/value.hpp
    lib/shader_graph/value.cpp
    lib/shader_graph/node/node.hpp
    lib/shader_graph/node/node.cpp
    lib/shader_graph/node/constant.hpp
    lib/shader_graph/node/constant.cpp
    lib/shader_graph/node/binary_operator.hpp
    lib/shader_graph/node/outputs.hpp
    lib/shader_graph/node/outputs.cpp
    lib/shader_graph/node/inputs.hpp
    lib/shader_graph/node/inputs.cpp
    lib/shader_graph/node/normalize.hpp
    lib/shader_graph/node/normalize.cpp
    lib/shader_graph/node/intersect.hpp
    lib/shader_graph/node/intersect.cpp
    lib/shader_graph/node/store_pixel.hpp
    lib/shader_graph/node/store_pixel.cpp
    lib/shader_graph/node/fetch_primitive.hpp
    lib/shader_graph/node/fetch_primitive.cpp
    lib/shader_graph/node/mix.hpp
    lib/shader_graph/node/mix.cpp
    lib/shader_graph/node/smooth_step.hpp
    lib/shader_graph/node/smooth_step.cpp
    lib/shader_graph/instructions.hpp
    lib/shader_graph/instructions.cpp
    lib/shader_graph/overload.hpp
    lib/shader_graph/overload.cpp
    lib/shader_graph/node/constructor.hpp
    lib/shader_graph/node/constructor.cpp
    lib/shader_graph/constructor_cache.hpp
    lib/shader_graph/constructor_cache.cpp
    lib/shader_graph/document.hpp
    lib/shader_graph/document.cpp

    lib/vulkan/device.hpp
    lib/vulkan/device.cpp
    lib/vulkan/index_buffer.hpp
    lib/vulkan/index_buffer.cpp
    lib/vulkan/vertex_buffer.hpp
    lib/vulkan/vertex_buffer.cpp
    lib/vulkan/frame_buffer.hpp
    lib/vulkan/frame_buffer.cpp
    lib/vulkan/command_buffer.hpp
    lib/vulkan/command_buffer.cpp
    lib/vulkan/bottom_level_acceleration_structure.hpp
    lib/vulkan/bottom_level_acceleration_structure.cpp
)

set_target_properties(rtintersect PROPERTIES LINKER_LANGUAGE CXX)
include_directories(rtintersect PUBLIC dependencies/glm dependencies)
target_link_libraries(rtintersect Vulkan::Vulkan)

# Create executable target
add_executable(RtTrace
    src/shaders.cpp
    src/main.cpp
)

# Link executable target with library target
target_include_directories(RtTrace PRIVATE lib)
target_link_libraries(RtTrace rtintersect)
