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

Result<Void> VulkanCommandBuffer::build_bottom_level_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
                                                                            const VertexBuffer* const vertex_buffer,
                                                                            size_t vertex_stride,
                                                                            const IndexBuffer* const index_buffer)
{
	return Result<Void>::from_error("Not implemented");
}

Result<Void> VulkanCommandBuffer::build_top_level_acceleration_structure(TopLevelAccelerationStructure* const acceleration_structure,
                                                                         const Instance* const instances,
                                                                         size_t count)
{
	return Result<Void>::from_error("Not implemented");
}

Result<Void> VulkanCommandBuffer::trace_rays(ResourceContext* resource_ctx, uint32_t x, uint32_t y)
{
	return Result<Void>::from_error("Not implemented");
}
