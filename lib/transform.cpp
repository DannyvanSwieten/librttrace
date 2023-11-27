#include "transform.hpp"
#include <cmath>

Transform::Transform()
{}

Transform::Transform(const Float3& translation, const Float3& rotation, const Float3& scale) : m_translation(translation), m_rotation(rotation), m_scale(scale)
{}

InstanceTransform Transform::get_instance_transform() const
{
	InstanceTransform instance_transform;
	instance_transform.transform[0] = m_scale.x * m_rotation.x;
	instance_transform.transform[1] = m_scale.x * m_rotation.y;
	instance_transform.transform[2] = m_scale.x * m_rotation.z;
	instance_transform.transform[3] = m_translation.x;
	instance_transform.transform[4] = m_scale.y * m_rotation.x;
	instance_transform.transform[5] = m_scale.y * m_rotation.y;
	instance_transform.transform[6] = m_scale.y * m_rotation.z;
	instance_transform.transform[7] = m_translation.y;
	instance_transform.transform[8] = m_scale.z * m_rotation.x;
	instance_transform.transform[9] = m_scale.z * m_rotation.y;
	instance_transform.transform[10] = m_scale.z * m_rotation.z;
	instance_transform.transform[11] = m_translation.z;
	return instance_transform;
}

InstanceTransform operator*(const InstanceTransform& lhs, const InstanceTransform& rhs)
{
	InstanceTransform result;
	for (size_t row = 0; row < InstanceTransform::ROWS; ++row)
	{
		for (size_t column = 0; column < InstanceTransform::COLUMNS; ++column)
		{
			result.transform[row * InstanceTransform::COLUMNS + column] =
				lhs.transform[row * InstanceTransform::COLUMNS + 0] * rhs.transform[0 * InstanceTransform::COLUMNS + column] +
				lhs.transform[row * InstanceTransform::COLUMNS + 1] * rhs.transform[1 * InstanceTransform::COLUMNS + column] +
				lhs.transform[row * InstanceTransform::COLUMNS + 2] * rhs.transform[2 * InstanceTransform::COLUMNS + column];
		}
	}
	return result;
}

InstanceTransform quat_to_transform(const Quaternion& q)
{
	InstanceTransform result;
	result.transform[0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	result.transform[1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	result.transform[2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
	result.transform[3] = 0.0f;
	result.transform[4] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	result.transform[5] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	result.transform[6] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
	result.transform[7] = 0.0f;
	result.transform[8] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	result.transform[9] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	result.transform[10] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
	result.transform[11] = 0.0f;
	return result;
}