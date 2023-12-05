#pragma once

#include <vulkan/vulkan.h>
#include "../bottom_level_acceleration_structure.hpp"

class VulkanDevice;
class VertexBuffer;
class IndexBuffer;
class Device;
class VulkanBottomLevelAccelerationStructure : public BottomLevelAccelerationStructure
{
public:
	VulkanBottomLevelAccelerationStructure(Device& device,
	                                       const VertexBuffer* const vertex_buffer,
	                                       size_t vertex_stride,
	                                       const IndexBuffer* const index_buffer);

	Instance create_instance(uint32_t instance_id, InstanceMask mask, const Mat4x4& transform) override;
};