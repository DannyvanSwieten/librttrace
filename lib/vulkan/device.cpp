#include <cassert>
#include <vector>
#include "device.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "frame_buffer.hpp"

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

	auto result = vkCreateInstance(&createInfo, nullptr, &m_instance);
	assert(result == VK_SUCCESS);

	uint32_t physical_device_count;
	result = vkEnumeratePhysicalDevices(m_instance, &physical_device_count, nullptr);
	assert(result == VK_SUCCESS);
	std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
	result = vkEnumeratePhysicalDevices(m_instance, &physical_device_count, physical_devices.data());
	assert(result == VK_SUCCESS);
	for (auto& physical_device : physical_devices)
	{
		vkGetPhysicalDeviceProperties(physical_device, &m_physical_device_properties);
		if (m_physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			m_physical_device = physical_device;
			break;
		}
	}

	vkGetPhysicalDeviceMemoryProperties(m_physical_device, &m_physical_device_memory_properties);

	uint32_t queue_family_count;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_families.data());
	for (auto& queue_family : queue_families)
	{
		if (queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			break;
		}
		m_compute_queue_family_index++;
	}

	float queue_priority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = m_compute_queue_family_index;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queue_priority;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

	result = vkCreateDevice(m_physical_device, &deviceCreateInfo, nullptr, &m_device);
	assert(result == VK_SUCCESS);

	vkGetDeviceQueue(m_device, m_compute_queue_family_index, 0, &m_queue);

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = m_compute_queue_family_index;

	result = vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_command_pool);
	assert(result == VK_SUCCESS);
}

VulkanDevice::~VulkanDevice()
{
	vkDestroyCommandPool(m_device, m_command_pool, nullptr);
	vkDestroyDevice(m_device, nullptr);
	vkDestroyInstance(m_instance, nullptr);
}

Result<VertexBuffer*> VulkanDevice::alloc_vertex_buffer(const float* const data, size_t size)
{
	return Result<VertexBuffer*>::from_value(new VulkanVertexBuffer(*this, data, size));
}

Result<IndexBuffer*> VulkanDevice::alloc_index_buffer(const uint32_t* const data, size_t count)
{
	return Result<IndexBuffer*>::from_value(new VulkanIndexBuffer(m_device, data, count));
}

Result<BottomLevelAccelerationStructure*>
VulkanDevice::alloc_acceleration_structure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer)
{
	return Result<BottomLevelAccelerationStructure*>::from_error("Not implemented");
}

Result<Void> VulkanDevice::build_acceleration_structure(CommandBuffer* command_buffer,
                                                        BottomLevelAccelerationStructure* acceleration_structure,
                                                        const VertexBuffer* const vertex_buffer,
                                                        size_t vertex_stride,
                                                        const IndexBuffer* const index_buffer)
{
	return Result<Void>::from_error("Not implemented");
}

Result<TopLevelAccelerationStructure*>
VulkanDevice::alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures, size_t count)
{
	return Result<TopLevelAccelerationStructure*>::from_error("Not implemented");
}

Result<Void> VulkanDevice::build_acceleration_structure(CommandBuffer* command_buffer,
                                                        TopLevelAccelerationStructure* tlas,
                                                        const BottomLevelAccelerationStructure* const acceleration_structures,
                                                        size_t count)
{
	return Result<Void>::from_error("Not implemented");
}

Result<FrameBuffer*> VulkanDevice::alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height)
{
	return Result<FrameBuffer*>::from_value(new VulkanFrameBuffer(*this, format, width, height));
}

Result<const char*> VulkanDevice::vendor_id() const
{
	return Result<const char*>::from_error("Not implemented");
}

uint32_t VulkanDevice::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const
{
	for (uint32_t i = 0; i < m_physical_device_memory_properties.memoryTypeCount; i++)
	{
		if ((type_filter & (1 << i)) && (m_physical_device_memory_properties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	assert(false);
	return 0;
}
