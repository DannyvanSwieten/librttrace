#pragma once

#include <vulkan/vulkan.h>

#include "../index_buffer.hpp"

class VulkanIndexBuffer: public IndexBuffer
{
public:
    VulkanIndexBuffer(VkDevice device, const uint32_t* const data, size_t size);
    ~VulkanIndexBuffer();

    VkBuffer getBuffer() const;
    VkDeviceMemory getDeviceMemory() const;
};