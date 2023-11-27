#pragma once
#include "result.hpp"
class Intersector;
class VertexBuffer;
class IndexBuffer;
class BottomLevelAccelerationStructure;
class TopLevelAccelerationStructure;
class ResourceContext;
struct Instance;
class CommandBuffer
{
public:
	virtual ~CommandBuffer() = 0;

	virtual Result<Void> build_bottom_level_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
	                                                               const VertexBuffer* const vertex_buffer,
	                                                               size_t vertex_stride,
	                                                               const IndexBuffer* const index_buffer) = 0;

	virtual Result<Void>
	build_top_level_acceleration_structure(TopLevelAccelerationStructure* const acceleration_structure, const Instance* const instances, size_t count) = 0;

	virtual Result<Void> trace_rays(ResourceContext* resource_ctx, uint32_t x, uint32_t y) = 0;
};