#pragma once
#include <vector>
#include "../float3.hpp"
#include "../top_level_acceleration_structure.hpp"
#include "intersect.hpp"

class CpuBottomLevelAccelerationStructure;
struct Instance;
class CpuTopLevelAccelerationStructure : public TopLevelAccelerationStructure
{
public:
	CpuTopLevelAccelerationStructure(const Instance* const acceleration_structures, size_t count);
	~CpuTopLevelAccelerationStructure() override;

	HitRecord intersect(const Float3& origin, const Float3& direction) const;

private:
	std::vector<Instance> m_instances;
	BoundingBox m_bounding_box;
};