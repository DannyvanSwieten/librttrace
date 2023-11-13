#include "vertex_buffer.hpp"

VulkanVertexBuffer::VulkanVertexBuffer(VkDevice device, const float* const data, size_t size)
{
    VkBufferCreateInfo bufferInfo = {};
    vkCreateBuffer(device, &bufferInfo, nullptr, &m_buffer);
}