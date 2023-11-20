#pragma once

#include "../device.hpp"

class CpuDevice : public Device
{
public:
	CpuDevice();
	Result<VertexBuffer*> alloc_vertex_buffer(const float* const data, size_t size) override;
	Result<IndexBuffer*> alloc_index_buffer(const uint32_t* const data, size_t count) override;

	Result<BottomLevelAccelerationStructure*>
	alloc_acceleration_structure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer) override;

	Result<TopLevelAccelerationStructure*> alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures,
	                                                                              size_t count) override;

	Result<FrameBuffer*> alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height) override;

	Result<CommandBuffer*> alloc_command_buffer() override;

	Result<const char*> vendor_id() const override;

private:
	std::string m_vendor_id;
};