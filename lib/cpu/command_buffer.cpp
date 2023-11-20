#include "command_buffer.hpp"
#include "bottom_level_acceleration_structure.hpp"

Result<Void> CpuCommandBuffer::build_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
                                                            const VertexBuffer* const vertex_buffer,
                                                            size_t vertex_stride,
                                                            const IndexBuffer* const index_buffer)
{
	auto cpu_acceleration_structure = static_cast<CpuBottomLevelAccelerationStructure* const>(acceleration_structure);
	cpu_acceleration_structure->build(vertex_buffer, vertex_stride, index_buffer);
	return Result<Void>::from_value(Void());
}

Result<Void> CpuCommandBuffer::intersect(const BottomLevelAccelerationStructure* const acceleration_structure)
{
	return Result<Void>::from_value(Void());
}
