#pragma once

#include <vulkan/vulkan.h>

#include "../index_buffer.hpp"

class VulkanDevice;
class VulkanIndexBuffer : public IndexBuffer
{
public:
	VulkanIndexBuffer(VulkanDevice& device, const uint32_t* const data, size_t size);
	~VulkanIndexBuffer();

	size_t size() const override
	{
		return m_memory_requirements.size;
	}

	VkDeviceAddress device_address() const
	{
		return m_device_address;
	}

	VkBuffer handle() const;
	VkDeviceMemory device_memory() const;

private:
	VulkanDevice& m_device;
	VkBuffer m_buffer;
	VkDeviceMemory m_memory;
	VkMemoryRequirements m_memory_requirements;
	VkDeviceAddress m_device_address;
};