#pragma once
#include <array>
#include <cstdint>
#include "float3.hpp"

class BottomLevelAccelerationStructure;
class IndexBuffer;
class VertexBuffer;

enum class InstanceMask
{
	Opaque = 0x01,
};

struct Instance
{
	const IndexBuffer* m_index_buffer;
	std::array<const VertexBuffer*, 4> m_attribute_buffers;
	BottomLevelAccelerationStructure* m_acceleration_structure;
	uint32_t m_instance_id;
	InstanceMask m_mask;
	Mat4x4 m_transform;
};