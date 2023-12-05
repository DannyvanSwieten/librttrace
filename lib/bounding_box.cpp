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
	return 2.0f * (s[0] * s[1] + s[0] * s[2] + s[1] * s[2]);
}

float BoundingBox::volume() const
{
	const auto s = size();
	return s[0] * s[1] * s[2];
}

BoundingBox BoundingBox::transformed(const Mat4x4& transform) const
{
	const auto c = center();
	const auto s = size();
	const auto x = transform * Float4(c[0] - s[0] * 0.5f, c[1] - s[1] * 0.5f, c[2] - s[2] * 0.5f, 1.0f);
	const auto y = transform * Float4(c[0] + s[0] * 0.5f, c[1] - s[1] * 0.5f, c[2] - s[2] * 0.5f, 1.0f);
	const auto z = transform * Float4(c[0] - s[0] * 0.5f, c[1] + s[1] * 0.5f, c[2] - s[2] * 0.5f, 1.0f);
	const auto w = transform * Float4(c[0] - s[0] * 0.5f, c[1] - s[1] * 0.5f, c[2] + s[2] * 0.5f, 1.0f);
	const auto v = transform * Float4(c[0] + s[0] * 0.5f, c[1] + s[1] * 0.5f, c[2] - s[2] * 0.5f, 1.0f);
	const auto u = transform * Float4(c[0] - s[0] * 0.5f, c[1] + s[1] * 0.5f, c[2] + s[2] * 0.5f, 1.0f);
	const auto t = transform * Float4(c[0] + s[0] * 0.5f, c[1] - s[1] * 0.5f, c[2] + s[2] * 0.5f, 1.0f);
	const auto r = transform * Float4(c[0] + s[0] * 0.5f, c[1] + s[1] * 0.5f, c[2] + s[2] * 0.5f, 1.0f);

	BoundingBox result;
	result.grow(Float3(x));
	result.grow(Float3(y));
	result.grow(Float3(z));
	result.grow(Float3(w));
	result.grow(Float3(v));
	result.grow(Float3(u));
	result.grow(Float3(t));
	result.grow(Float3(r));
	return result;
}
