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

	Result<BottomLevelAccelerationStructure*>
	alloc_acceleration_structure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer);

	Result<TopLevelAccelerationStructure*> alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures,
	                                                                              size_t count);
	Result<FrameBuffer*> alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height);

	Result<CommandBuffer*> alloc_command_buffer();

	Result<const char*> vendor_id() const;

	uint32_t compute_queue_family_index() const
	{
		return m_compute_queue_family_index;
	}
	VkQueue queue() const
	{
		return m_queue;
	}
	VkCommandPool command_pool() const
	{
		return m_command_pool;
	}
	VkDevice device() const
	{
		return m_device;
	}

	uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const;

private:
	VkInstance m_instance;
	VkDevice m_device;
	VkPhysicalDevice m_physical_device;
	VkPhysicalDeviceProperties2 m_physical_device_properties_2 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
	VkPhysicalDeviceMemoryProperties2 m_physical_device_memory_properties_2 = {};
	uint32_t m_compute_queue_family_index = 0;

	VkQueue m_queue;
	VkCommandPool m_command_pool;
	VkDescriptorPool m_descriptor_pool;
};