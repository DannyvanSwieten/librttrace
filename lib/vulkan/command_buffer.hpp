#pragma once
#include <vulkan/vulkan.h>
#include "../command_buffer.hpp"

class VulkanDevice;
class VulkanCommandBuffer : public CommandBuffer
{
public:
	VulkanCommandBuffer(VulkanDevice& device);
	Result<Void> build_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
	                                          const VertexBuffer* const vertex_buffer,
	                                          size_t vertex_stride,
	                                          const IndexBuffer* const index_buffer) override;
	Result<Void> intersect(const BottomLevelAccelerationStructure* const acceleration_structure) override;

private:
	VulkanDevice& m_device;
	VkCommandBuffer m_command_buffer = nullptr;
};