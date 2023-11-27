#include "top_level_acceleration_structure.hpp"
#include "../instance.hpp"
#include "bottom_level_acceleration_structure.hpp"
CpuTopLevelAccelerationStructure::CpuTopLevelAccelerationStructure(const Instance* const instances, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		const auto& instance = instances[i];
		const auto cpu_acceleration_structure = static_cast<const CpuBottomLevelAccelerationStructure* const>(instance.acceleration_structure);
		m_bounding_box.grow(cpu_acceleration_structure->bounding_box());
		m_instances.push_back(instances[i]);
	}
}

CpuTopLevelAccelerationStructure::~CpuTopLevelAccelerationStructure()
{}

HitRecord CpuTopLevelAccelerationStructure::intersect(const Float3& origin, const Float3& direction) const
{
	HitRecord record;
	float t_min = 0.01f;
	if (!intersect_bounding_box(origin, direction, m_bounding_box, t_min, record.t))
	{
		return record;
	}

	for (const auto& instance : m_instances)
	{
		const auto cpu_acceleration_structure = static_cast<const CpuBottomLevelAccelerationStructure* const>(instance.acceleration_structure);
		cpu_acceleration_structure->intersect(origin, direction, 0.01f, record);
	}

	return record;
}
