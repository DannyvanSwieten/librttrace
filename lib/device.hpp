#pragma once

#include "result.hpp"
#include "pixel_format.hpp"
#include "frame_buffer.hpp"

class VertexBuffer;
class IndexBuffer;
class BottomLevelAccelerationStructure;
class TopLevelAccelerationStructure;
class CommandBuffer;

class Device
{
public:
	virtual Result<VertexBuffer*> alloc_vertex_buffer(const float* const data, size_t size) = 0;
	virtual Result<IndexBuffer*> alloc_index_buffer(const uint32_t* const data, size_t count) = 0;

	virtual Result<BottomLevelAccelerationStructure*> alloc_acceleration_structure(const VertexBuffer* const vertex_buffer,
	                                                                               const IndexBuffer* const index_buffer) = 0;
	virtual Result<Void> build_acceleration_structure(CommandBuffer* command_buffer,
	                                                  BottomLevelAccelerationStructure* acceleration_structure,
	                                                  const VertexBuffer* const vertex_buffer,
	                                                  size_t vertex_stride,
	                                                  const IndexBuffer* const index_buffer) = 0;

	virtual Result<TopLevelAccelerationStructure*> alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures,
	                                                                                      size_t count) = 0;
	virtual Result<Void> build_acceleration_structure(CommandBuffer* command_buffer,
	                                                  TopLevelAccelerationStructure* tlas,
	                                                  const BottomLevelAccelerationStructure* const acceleration_structures,
	                                                  size_t count) = 0;

	virtual Result<FrameBuffer*> alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height) = 0;

	virtual Result<const char*> vendor_id() const = 0;
};
