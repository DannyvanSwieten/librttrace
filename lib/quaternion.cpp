#include "quaternion.hpp"
#include <cmath>

Quaternion rotate(const Quaternion& q, const Float3& axis, float angle)
{
	Quaternion result;
	float half_angle = angle * 0.5f;
	float sin_half_angle = std::sin(half_angle);
	result.x = axis[0] * sin_half_angle;
	result.y = axis[1] * sin_half_angle;
	result.z = axis[2] * sin_half_angle;
	result.w = std::cos(half_angle);
	return result * q;
}

Quaternion normalize(const Quaternion& q)
{
	float length = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	Quaternion result;
	result.x = q.x / length;
	result.y = q.y / length;
	result.z = q.z / length;
	result.w = q.w / length;
	return result;
}

Quaternion conjugate(const Quaternion& q)
{
	Quaternion result;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w = q.w;
	return result;
}

Quaternion inverse(const Quaternion& q)
{
	return conjugate(q) / Quaternion(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

Quaternion slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	float cos_half_theta = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
	if (std::abs(cos_half_theta) >= 1.0f)
	{
		return q0;
	}
	float half_theta = std::acos(cos_half_theta);
	float sin_half_theta = std::sqrt(1.0f - cos_half_theta * cos_half_theta);
	if (std::abs(sin_half_theta) < 0.001f)
	{
		Quaternion result;
		result.x = (q0.x * 0.5f + q1.x * 0.5f);
		result.y = (q0.y * 0.5f + q1.y * 0.5f);
		result.z = (q0.z * 0.5f + q1.z * 0.5f);
		result.w = (q0.w * 0.5f + q1.w * 0.5f);
		return result;
	}
	float ratio_a = std::sin((1.0f - t) * half_theta) / sin_half_theta;
	float ratio_b = std::sin(t * half_theta) / sin_half_theta;
	Quaternion result;
	result.x = (q0.x * ratio_a + q1.x * ratio_b);
	result.y = (q0.y * ratio_a + q1.y * ratio_b);
	result.z = (q0.z * ratio_a + q1.z * ratio_b);
	result.w = (q0.w * ratio_a + q1.w * ratio_b);
	return result;
}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	return result;
}

Quaternion operator/(const Quaternion& lhs, const Quaternion& rhs)
{
	return lhs * inverse(rhs);
}
