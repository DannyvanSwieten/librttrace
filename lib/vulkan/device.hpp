#pragma once

#include "../device.hpp"

#include <vulkan/vulkan.h>

class VulkanDevice : public Device
{
public:
	VulkanDevice();
	~VulkanDevice();

    Result<VertexBuffer*> alloc_vertex_buffer(const float* const data, size_t size);
	Result<IndexBuffer*> alloc_index_buffer(const uint32_t* const data, size_t count);

	Result<BottomLevelAccelerationStructure*> alloc_acceleration_structure(const VertexBuffer* const vertex_buffer,
	                                                                               const IndexBuffer* const index_buffer);
	Result<Void> build_acceleration_structure(CommandBuffer* command_buffer,
	                                                  BottomLevelAccelerationStructure* acceleration_structure,
	                                                  const VertexBuffer* const vertex_buffer,
	                                                  size_t vertex_stride,
	                                                  const IndexBuffer* const index_buffer);

	Result<TopLevelAccelerationStructure*> alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures,
	                                                                                      size_t count);
	Result<Void> build_acceleration_structure(CommandBuffer* command_buffer,
	                                                  TopLevelAccelerationStructure* tlas,
	                                                  const BottomLevelAccelerationStructure* const acceleration_structures,
	                                                  size_t count);

	Result<FrameBuffer*> alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height);

	Result<const char*> vendor_id() const;

private:
    VkInstance m_instance;
	VkDevice m_device;
};