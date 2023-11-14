#pragma once

#include <vulkan/vulkan.h>

#include "../index_buffer.hpp"

class VulkanIndexBuffer: public IndexBuffer
{
public:
    VulkanIndexBuffer(VkDevice device, const uint32_t* const data, size_t size);
    ~VulkanIndexBuffer();

    size_t size() const override
    {
        return 0;
    }

    VkBuffer getBuffer() const;
    VkDeviceMemory getDeviceMemory() const;
};