#include <cassert>
#include "device.hpp"
#include "frame_buffer.hpp"

VulkanFrameBuffer::VulkanFrameBuffer(VulkanDevice& device, PixelFormat format, uint32_t width, uint32_t height)
	: m_device(device)
	, m_width(width)
	, m_height(height)
{
	VkImageCreateInfo image_create_info = {};
	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_create_info.imageType = VK_IMAGE_TYPE_2D;
	image_create_info.format = format == PixelFormat::RGBA32SFLOAT ? VK_FORMAT_R32G32B32A32_SFLOAT : VK_FORMAT_R8G8B8A8_UNORM;
	image_create_info.extent.width = width;
	image_create_info.extent.height = height;
	image_create_info.extent.depth = 1;
	image_create_info.mipLevels = 1;
	image_create_info.arrayLayers = 1;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.tiling = VK_IMAGE_TILING_LINEAR;
	image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	auto result = vkCreateImage(device.device(), &image_create_info, nullptr, &m_image);
	assert(result == VK_SUCCESS);

	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(device.device(), m_image, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.allocationSize = memory_requirements.size;
	memory_allocate_info.memoryTypeIndex =
		device.find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkAllocateMemory(device.device(), &memory_allocate_info, nullptr, &m_memory);
	vkBindImageMemory(device.device(), m_image, m_memory, 0);

	VkImageViewCreateInfo image_view_create_info = {};
	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.image = m_image;
	image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_create_info.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_create_info.subresourceRange.baseMipLevel = 0;
	image_view_create_info.subresourceRange.levelCount = 1;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;

    result = vkCreateImageView(device.device(), &image_view_create_info, nullptr, &m_image_view);
    assert(result == VK_SUCCESS);
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
	vkDestroyImage(m_device.device(), m_image, nullptr);
	vkFreeMemory(m_device.device(), m_memory, nullptr);
}

void VulkanFrameBuffer::set_pixel(uint32_t x, uint32_t y, float r, float g, float b, float a)
{}

void VulkanFrameBuffer::clear()
{}

Result<const void*> VulkanFrameBuffer::data()
{
	return Result<const void*>::from_error("Not implemented");
}

uint32_t VulkanFrameBuffer::width()
{
	return m_width;
}

uint32_t VulkanFrameBuffer::height()
{
	return m_height;
}
