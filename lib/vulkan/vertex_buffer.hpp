#pragma once

#include <vulkan/vulkan.h>
#include "../vertex_buffer.hpp"

class VulkanDevice;
class VulkanVertexBuffer : public VertexBuffer
{
public:
	VulkanVertexBuffer(VulkanDevice& device, const float* const data, size_t size);
	~VulkanVertexBuffer();

	size_t size() const override
	{
		return m_memory_requirements.size;
	}

	VkBuffer handle() const
	{
		return m_buffer;
	}

	VkDeviceAddress device_address() const
	{
		return m_device_address;
	}

private:
	VulkanDevice& m_device;
	VkBuffer m_buffer;
	VkDeviceMemory m_memory;
	VkMemoryRequirements m_memory_requirements;
	VkDeviceAddress m_device_address;
};