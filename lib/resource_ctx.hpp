#pragma once
#include <array>
#include <vector>

class FrameBuffer;
class TopLevelAccelerationStructure;
class VertexBuffer;
class ResourceContext
{
public:
	void add_frame_buffer(size_t index, FrameBuffer* frame_buffer);
	FrameBuffer* get_frame_buffer(size_t index);

	void add_vertex_buffer(const VertexBuffer* vertex_buffer);
	const VertexBuffer* get_vertex_buffer(size_t index) const;

	void add_acceleration_structure(size_t index, const TopLevelAccelerationStructure* const acceleration_structure);
	const TopLevelAccelerationStructure* const get_acceleration_structure(size_t index);
	const std::array<const TopLevelAccelerationStructure*, 1>& acceleration_structures() const;

private:
	std::array<FrameBuffer*, 1> m_frame_buffers;
	std::array<const TopLevelAccelerationStructure*, 1> m_acceleration_structures;
	std::vector<const VertexBuffer*> m_vertex_buffers;
};