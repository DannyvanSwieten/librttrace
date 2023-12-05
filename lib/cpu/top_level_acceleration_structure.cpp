#include "top_level_acceleration_structure.hpp"
#include "../instance.hpp"
#include "bottom_level_acceleration_structure.hpp"
CpuTopLevelAccelerationStructure::CpuTopLevelAccelerationStructure(const Instance* const instances, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		const auto& instance = instances[i];
		const auto cpu_acceleration_structure = static_cast<const CpuBottomLevelAccelerationStructure* const>(instance.m_acceleration_structure);
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

	float t = std::numeric_limits<float>::max();

	for (const auto& instance : m_instances)
	{
		const auto inverse_transform = glm::inverse(instance.m_transform);
		const auto inverse_origin = inverse_transform * glm::vec4(origin, 1.0);
		const auto inverse_direction = normalize(inverse_transform * glm::vec4(direction, 0.0));

		const auto cpu_acceleration_structure = static_cast<const CpuBottomLevelAccelerationStructure* const>(instance.m_acceleration_structure);
		cpu_acceleration_structure->intersect(instance.m_transform, inverse_origin, inverse_direction, 0.01f, record);
		if (record.t < t)
		{
			t = record.t;
			record.instance_id = instance.m_instance_id;
		}
	}

	return record;
}

const Instance& CpuTopLevelAccelerationStructure::instance(size_t index) const
{
	return m_instances[index];
}
