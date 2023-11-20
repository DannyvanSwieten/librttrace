#include <array>
#include <cassert>
#include "vertex_buffer.hpp"
#include "device.hpp"

VulkanVertexBuffer::VulkanVertexBuffer(VulkanDevice& device, const float* const data, size_t size) : m_device(device)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
	                   VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.queueFamilyIndexCount = 1;
	std::array<uint32_t, 1> queue_family_indices = { device.compute_queue_family_index() };
	bufferInfo.pQueueFamilyIndices = queue_family_indices.data();

	auto result = vkCreateBuffer(device.device(), &bufferInfo, nullptr, &m_buffer);
	assert(result == VK_SUCCESS);

	vkGetBufferMemoryRequirements(device.device(), m_buffer, &m_memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info = {};
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.allocationSize = m_memory_requirements.size;
	memory_allocate_info.memoryTypeIndex =
		device.find_memory_type(m_memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	result = vkAllocateMemory(device.device(), &memory_allocate_info, nullptr, &m_memory);
	assert(result == VK_SUCCESS);

	auto device_address_info = VkBufferDeviceAddressInfo{ VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO };
	device_address_info.buffer = m_buffer;
	m_device_address = vkGetBufferDeviceAddress(m_device.device(), &device_address_info);
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	vkDestroyBuffer(m_device.device(), m_buffer, nullptr);
}
