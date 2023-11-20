#include <cassert>

#include "command_buffer.hpp"
#include "device.hpp"

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice& device) : m_device(device)
{
	auto vk_device = m_device.device();
	VkCommandBufferAllocateInfo command_buffer_allocate_info = {};
	command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool = m_device.command_pool();
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandBufferCount = 1;

	auto result = vkAllocateCommandBuffers(vk_device, &command_buffer_allocate_info, &m_command_buffer);
	assert(result == VK_SUCCESS);
}

Result<Void> VulkanCommandBuffer::build_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
                                                               const VertexBuffer* const vertex_buffer,
                                                               size_t vertex_stride,
                                                               const IndexBuffer* const index_buffer)
{
	return Result<Void>::from_error("Not implemented");
}

Result<Void> VulkanCommandBuffer::intersect(const BottomLevelAccelerationStructure* const acceleration_structure)
{
	return Result<Void>::from_error("Not implemented");
}
