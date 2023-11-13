#pragma once

#include <vulkan/vulkan.h>
#include "../vertex_buffer.hpp"

class VulkanVertexBuffer
{
public:
    VulkanVertexBuffer(VkDevice device, const float* const data, size_t size);
    ~VulkanVertexBuffer();
};