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
		return 0;
	}

private:
	VulkanDevice& m_device;
	VkBuffer m_buffer;
	VkDeviceMemory m_memory;
};