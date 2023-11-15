#pragma once

#include <vulkan/vulkan.h>

#include "../frame_buffer.hpp"

class VulkanDevice;
class VulkanFrameBuffer : public FrameBuffer
{
public:
    VulkanFrameBuffer(VulkanDevice& device, PixelFormat format, uint32_t width, uint32_t height);
    ~VulkanFrameBuffer();

    void set_pixel(uint32_t x, uint32_t y, float r, float g, float b, float a);
	void clear();

	Result<const void*> data();
	uint32_t width();
	uint32_t height();

    private:

    VulkanDevice& m_device;
    VkImage m_image;
    VkImageView m_image_view;
    VkDeviceMemory m_memory;
    uint32_t m_width;
    uint32_t m_height;
};