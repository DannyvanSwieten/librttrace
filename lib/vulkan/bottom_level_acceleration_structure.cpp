#include "bottom_level_acceleration_structure.hpp"

#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "device.hpp"

VulkanBottomLevelAccelerationStructure::VulkanBottomLevelAccelerationStructure(Device& device,
                                                                               const VertexBuffer* const vertex_buffer,
                                                                               size_t vertex_stride,
                                                                               const IndexBuffer* const index_buffer)
{
	auto& vulkan_device = static_cast<VulkanDevice&>(device);
	auto vk_device = vulkan_device.device();
	const auto vk_vertex_buffer = static_cast<const VulkanVertexBuffer&>(*vertex_buffer);
	const auto vk_index_buffer = static_cast<const VulkanIndexBuffer&>(*index_buffer);

	VkAccelerationStructureGeometryTrianglesDataKHR triangles_data = {};
	triangles_data.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
	triangles_data.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
	triangles_data.vertexData.deviceAddress = vk_vertex_buffer.device_address();
	triangles_data.vertexStride = vertex_stride * sizeof(float);
	triangles_data.indexType = VK_INDEX_TYPE_UINT32;
	triangles_data.indexData.deviceAddress = vk_index_buffer.device_address();
	triangles_data.maxVertex = vk_vertex_buffer.size() / vertex_stride;
}

Instance VulkanBottomLevelAccelerationStructure::create_instance(uint32_t instance_id, InstanceMask mask, const Mat4x4& transform)
{
	return Instance();
}
