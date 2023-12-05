#include "resource_ctx.hpp"

void ResourceContext::add_frame_buffer(size_t id, FrameBuffer* frame_buffer)
{
	m_frame_buffers[id] = frame_buffer;
}

FrameBuffer* ResourceContext::get_frame_buffer(size_t id)
{
	return m_frame_buffers[id];
}

void ResourceContext::add_vertex_buffer(const VertexBuffer* vertex_buffer)
{
	m_vertex_buffers.emplace_back(vertex_buffer);
}

const VertexBuffer* ResourceContext::get_vertex_buffer(size_t index) const
{
	return m_vertex_buffers[index];
}

void ResourceContext::add_acceleration_structure(size_t id, const TopLevelAccelerationStructure* const acceleration_structure)
{
	m_acceleration_structures[id] = acceleration_structure;
}

const TopLevelAccelerationStructure* const ResourceContext::get_acceleration_structure(size_t id)
{
	return m_acceleration_structures[id];
}

const std::array<const TopLevelAccelerationStructure*, 1>& ResourceContext::acceleration_structures() const
{
	return m_acceleration_structures;
}
