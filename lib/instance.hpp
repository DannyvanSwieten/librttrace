#pragma once
#include <cstdint>

class BottomLevelAccelerationStructure;

enum class InstanceMask
{
	Opaque = 0x01,
};

struct Instance
{
	BottomLevelAccelerationStructure* acceleration_structure;
	uint32_t instance_id;
	InstanceMask mask;
	float transform[12];
};