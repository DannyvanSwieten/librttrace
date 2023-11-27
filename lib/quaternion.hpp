#pragma once
#include "float3.hpp"
struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);
Quaternion operator/(const Quaternion& lhs, const Quaternion& rhs);
Quaternion rotate(const Quaternion& q, const Float3& axis, float angle);
Quaternion normalize(const Quaternion& q);
Quaternion conjugate(const Quaternion& q);
Quaternion inverse(const Quaternion& q);
Quaternion slerp(const Quaternion& q0, const Quaternion& q1, float t);