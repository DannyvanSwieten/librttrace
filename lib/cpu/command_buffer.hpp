#pragma once
#include "../command_buffer.hpp"

class CpuCommandBuffer : public CommandBuffer
{
public:
	Result<Void> build_bottom_level_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
	                                                       const VertexBuffer* const vertex_buffer,
	                                                       size_t vertex_stride,
	                                                       const IndexBuffer* const index_buffer) override;

	Result<Void>
	build_top_level_acceleration_structure(TopLevelAccelerationStructure* const acceleration_structure, const Instance* const instances, size_t count) override;

	Result<Void> trace_rays(const Pipeline* pipeline, ResourceContext* resource_ctx, uint32_t x, uint32_t y) override;
};