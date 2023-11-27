#include "bounding_box.hpp"
#include <numeric>

BoundingBox::BoundingBox() : m_min(1000000), m_max(-1000000)
{}

BoundingBox::BoundingBox(const Float3& min, const Float3& max) : m_min(min), m_max(max)
{}

BoundingBox& BoundingBox::grow(const Float3& point)
{
	m_min = min(m_min, point);
	m_max = max(m_max, point);
	return *this;
}

BoundingBox& BoundingBox::grow(const BoundingBox& other)
{
	m_min = min(m_min, other.m_min);
	m_max = max(m_max, other.m_max);
	return *this;
}

Float3 BoundingBox::center() const
{
	return (m_min + m_max) * 0.5f;
}

Float3 BoundingBox::size() const
{
	return m_max - m_min;
}

float BoundingBox::area() const
{
	const auto s = size();
	return 2.0f * (s.x * s.y + s.x * s.z + s.y * s.z);
}

float BoundingBox::volume() const
{
	const auto s = size();
	return s.x * s.y * s.z;
}
