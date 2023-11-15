#include <array>
#include "vertex_buffer.hpp"
#include "device.hpp"

VulkanVertexBuffer::VulkanVertexBuffer(VulkanDevice& device, const float* const data, size_t size) : m_device(device)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.queueFamilyIndexCount = 1;
	std::array<uint32_t, 1> queue_family_indices = { device.compute_queue_family_index() };
	bufferInfo.pQueueFamilyIndices = queue_family_indices.data();

	vkCreateBuffer(device.device(), &bufferInfo, nullptr, &m_buffer);

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device.device(), m_buffer, &memoryRequirements);

    VkMemoryAllocateInfo memory_allocate_info = {};
    memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memory_allocate_info.allocationSize = memoryRequirements.size;
    memory_allocate_info.memoryTypeIndex = 0;

    vkAllocateMemory(device.device(), &memory_allocate_info, nullptr, &m_memory);
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	vkDestroyBuffer(m_device.device(), m_buffer, nullptr);
}
