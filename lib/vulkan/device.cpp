#include "device.hpp"

VulkanDevice::VulkanDevice()
{
    VkApplicationInfo appInfo = {};
    appInfo.apiVersion = VK_MAKE_VERSION(1, 3, 0);
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pApplicationName = "RT Trace Vulkan";
    appInfo.pEngineName = "RT Trace Vulkan";

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    vkCreateInstance(&createInfo, nullptr, &m_instance);
}

VulkanDevice::~VulkanDevice()
{}

Result<VertexBuffer*> VulkanDevice::alloc_vertex_buffer(const float* const data, size_t size)
{
	return Result<VertexBuffer*>();
}

Result<IndexBuffer*> VulkanDevice::alloc_index_buffer(const uint32_t* const data, size_t count)
{
	return Result<IndexBuffer*>();
}

Result<BottomLevelAccelerationStructure*> VulkanDevice::alloc_acceleration_structure(const VertexBuffer* const vertex_buffer,
                                                                                     const IndexBuffer* const index_buffer)
{
	return Result<BottomLevelAccelerationStructure*>();
}

Result<Void> VulkanDevice::build_acceleration_structure(CommandBuffer* command_buffer,
                                                        BottomLevelAccelerationStructure* acceleration_structure,
                                                        const VertexBuffer* const vertex_buffer,
                                                        size_t vertex_stride,
                                                        const IndexBuffer* const index_buffer)
{
	return Result<Void>();
}

Result<TopLevelAccelerationStructure*>
VulkanDevice::alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures, size_t count)
{
	return Result<TopLevelAccelerationStructure*>();
}

Result<Void> VulkanDevice::build_acceleration_structure(CommandBuffer* command_buffer,
                                                        TopLevelAccelerationStructure* tlas,
                                                        const BottomLevelAccelerationStructure* const acceleration_structures,
                                                        size_t count)
{
	return Result<Void>();
}

Result<FrameBuffer*> VulkanDevice::alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height)
{
	return Result<FrameBuffer*>();
}

Result<const char*> VulkanDevice::vendor_id() const
{
	return Result<const char*>();
}
