#pragma once
#include <array>

class FrameBuffer;
class TopLevelAccelerationStructure;
class ResourceContext
{
public:
	void add_frame_buffer(size_t index, FrameBuffer* frame_buffer);
	FrameBuffer* get_frame_buffer(size_t index);

	void add_acceleration_structure(size_t index, const TopLevelAccelerationStructure* const acceleration_structure);
	const TopLevelAccelerationStructure* const get_acceleration_structure(size_t index);
	const std::array<const TopLevelAccelerationStructure*, 1>& acceleration_structures() const;

private:
	std::array<FrameBuffer*, 1> m_frame_buffers;
	std::array<const TopLevelAccelerationStructure*, 1> m_acceleration_structures;
};