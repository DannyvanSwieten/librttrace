#pragma once
#include "result.hpp"
class Intersector;
class VertexBuffer;
class IndexBuffer;
class BottomLevelAccelerationStructure;
class CommandBuffer
{
public:
	virtual ~CommandBuffer() = 0;

	virtual Result<Void> build_acceleration_structure(const Intersector& intersector) = 0;
	virtual Result<Void> intersect(BottomLevelAccelerationStructure* acceleration_structure,
	                               const VertexBuffer* const vertex_buffer,
	                               size_t vertex_stride,
	                               const IndexBuffer* const index_buffer) = 0;
};