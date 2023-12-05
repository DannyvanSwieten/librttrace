#pragma once

#include "float3.hpp"

struct BoundingBox
{
	Float3 m_min;
	Float3 m_max;

	BoundingBox();
	BoundingBox(const Float3& min, const Float3& max);

	BoundingBox& grow(const Float3& point);
	BoundingBox& grow(const BoundingBox& other);
	Float3 center() const;
	Float3 size() const;
	float area() const;
	float volume() const;
	BoundingBox transformed(const Mat4x4& transform) const;
};
