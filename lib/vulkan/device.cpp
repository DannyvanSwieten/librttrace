#include <array>
#include <cassert>
#include <vector>
#include "device.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "frame_buffer.hpp"
#include "bottom_level_acceleration_structure.hpp"
#include "command_buffer.hpp"

VulkanDevice::VulkanDevice()
{
	m_physical_device_memory_properties_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
	m_physical_device_properties_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;

	VkApplicationInfo appInfo = {};
	appInfo.apiVersion = VK_MAKE_VERSION(1, 3, 0);
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pApplicationName = "RT Trace Vulkan";
	appInfo.pEngineName = "RT Trace Vulkan";

	std::array<const char*, 4> extensions = { VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
		                                      VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
		                                      VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		                                      VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME };

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
		vkGetPhysicalDeviceProperties2(physical_device, &m_physical_device_properties_2);
		if (m_physical_device_properties_2.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			m_physical_device = physical_device;
			break;
		}
	}

	uint32_t count = 0;
	vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &count, nullptr);
	std::vector<VkExtensionProperties> device_extension_properties(count);
	vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &count, device_extension_properties.data());

	vkGetPhysicalDeviceMemoryProperties2(m_physical_device, &m_physical_device_memory_properties_2);

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

	VkPhysicalDeviceRayTracingPipelineFeaturesKHR ray_tracing_pipeline_features = {};
	ray_tracing_pipeline_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
	ray_tracing_pipeline_features.rayTracingPipeline = VK_TRUE;

	VkPhysicalDeviceAccelerationStructureFeaturesKHR acceleration_structure_features = {};
	acceleration_structure_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
	acceleration_structure_features.accelerationStructure = VK_TRUE;
	acceleration_structure_features.pNext = &ray_tracing_pipeline_features;

	VkPhysicalDeviceBufferDeviceAddressFeatures buffer_device_address_features = {};
	buffer_device_address_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
	buffer_device_address_features.bufferDeviceAddress = VK_TRUE;
	buffer_device_address_features.pNext = &acceleration_structure_features;

	VkPhysicalDeviceFeatures2 device_features = {};
	device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	device_features.pNext = &buffer_device_address_features;

	vkGetPhysicalDeviceFeatures2(m_physical_device, &device_features);

	device_features.pNext = &buffer_device_address_features;
	device_features.features.samplerAnisotropy = VK_TRUE;
	device_features.features.shaderInt64 = VK_TRUE;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
	deviceCreateInfo.pEnabledFeatures = &device_features.features;

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
	return Result<IndexBuffer*>::from_value(new VulkanIndexBuffer(*this, data, count));
}

Result<BottomLevelAccelerationStructure*>
VulkanDevice::alloc_acceleration_structure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer)
{
	return Result<BottomLevelAccelerationStructure*>::from_value(new VulkanBottomLevelAccelerationStructure(*this, vertex_buffer, vertex_stride, index_buffer));
}

Result<TopLevelAccelerationStructure*>
VulkanDevice::alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures, size_t count)
{
	return Result<TopLevelAccelerationStructure*>::from_error("Not implemented");
}

Result<FrameBuffer*> VulkanDevice::alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height)
{
	return Result<FrameBuffer*>::from_value(new VulkanFrameBuffer(*this, format, width, height));
}

Result<CommandBuffer*> VulkanDevice::alloc_command_buffer()
{
	return Result<CommandBuffer*>::from_value(new VulkanCommandBuffer(*this));
}

Result<const char*> VulkanDevice::vendor_id() const
{
	return Result<const char*>::from_error("Not implemented");
}

uint32_t VulkanDevice::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) const
{
	for (uint32_t i = 0; i < m_physical_device_memory_properties_2.memoryProperties.memoryTypeCount; i++)
	{
		if ((type_filter & (1 << i)) && (m_physical_device_memory_properties_2.memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	assert(false);
	return 0;
}
