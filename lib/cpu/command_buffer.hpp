#pragma once
#include "../command_buffer.hpp"

class CpuCommandBuffer : public CommandBuffer
{
public:
	Result<Void> build_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
	                                          const VertexBuffer* const vertex_buffer,
	                                          size_t vertex_stride,
	                                          const IndexBuffer* const index_buffer) override;
	Result<Void> intersect(const BottomLevelAccelerationStructure* const acceleration_structure) override;
};