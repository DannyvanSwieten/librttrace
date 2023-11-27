#pragma once
#include <map>

class FrameBuffer;
class TopLevelAccelerationStructure;
class ResourceContext
{
public:
	void add_frame_buffer(size_t id, FrameBuffer* frame_buffer);
	FrameBuffer* get_frame_buffer(size_t id);

	void add_acceleration_structure(size_t id, const TopLevelAccelerationStructure* const acceleration_structure);
	const TopLevelAccelerationStructure* const get_acceleration_structure(size_t id);
	const std::map<size_t, const TopLevelAccelerationStructure*>& acceleration_structures() const;

private:
	std::map<size_t, FrameBuffer*> m_frame_buffers;
	std::map<size_t, const TopLevelAccelerationStructure*> m_acceleration_structures;
};