#pragma once

#include <vulkan/vulkan.h>
#include "../vertex_buffer.hpp"

class VulkanVertexBuffer : public VertexBuffer
{
public:
	VulkanVertexBuffer(VkDevice device, const float* const data, size_t size);
	~VulkanVertexBuffer();

	size_t size() const override
	{
		return 0;
	}

private:
	VkBuffer m_buffer;
};