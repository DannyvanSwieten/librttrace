#pragma once

#include "instance.hpp"
class BottomLevelAccelerationStructure
{
public:
	virtual ~BottomLevelAccelerationStructure() = 0;
	virtual Instance create_instance(uint32_t instance_id, InstanceMask mask, const Mat4x4& transform = glm::identity<Mat4x4>()) = 0;
};