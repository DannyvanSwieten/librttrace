#include "Float3.hpp"
#include <cmath>

Float3 operator+(const Float3& a, const Float3& b)
{
	auto result = _mm_add_ps(a.data.data, b.data.data);
	return { result };
}

Float3 operator-(const Float3& a, const Float3& b)
{
	return { _mm_sub_ps(a.data.data, b.data.data) };
}

Float3 operator*(const Float3& a, const Float3& b)
{
	return { _mm_mul_ps(a.data.data, b.data.data) };
}

Float3 operator*(const Float3& a, float b)
{
	return { _mm_mul_ps(a.data.data, _mm_set_ps(1.0, b, b, b)) };
}

Float3 operator/(const Float3& a, const Float3& b)
{
	return { _mm_div_ps(a.data.data, b.data.data) };
}

Float3 operator/(const Float3& a, float b)
{
	return { _mm_div_ps(a.data.data, _mm_set_ps(1.0, b, b, b)) };
}

float dot(const Float3& a, const Float3& b)
{
	// auto result = Float3{ _mm_dp_ps(a.data.data, b.data.data, 0xFF) };
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

Float3 cross(const Float3& a, const Float3& b)
{
	return { a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] };
}

Float3 normalize(const Float3& a)
{
	return a / std::sqrt(dot(a, a));
}

Float3 reflect(const Float3& a, const Float3& b)
{
	return a - 2 * dot(a, b) * b;
}

Float3 refract(const Float3& a, const Float3& b, float eta)
{
	Float3 n = normalize(b);
	float cosi = dot(a, n);
	float cost2 = 1 - eta * eta * (1 - cosi * cosi);
	if (cost2 < 0)
		return { 0, 0, 0 };
	float cost = std::sqrt(cost2);
	return eta * a - (eta * cosi + cost) * n;
}

Float3 pow(const Float3& a, float b)
{
	return { std::pow(a[0], b), std::pow(a[1], b), std::pow(a[2], b) };
}

Float3 sqrt(const Float3& a)
{
	return { std::sqrt(a[0]), std::sqrt(a[1]), std::sqrt(a[2]) };
}

Float3 abs(const Float3& a)
{
	return { std::abs(a[0]), std::abs(a[1]), std::abs(a[2]) };
}

Float3 min(const Float3& a, const Float3& b)
{
	return _mm_min_ps(a.data.data, b.data.data);
}

Float3 max(const Float3& a, const Float3& b)
{
	return _mm_max_ps(a.data.data, b.data.data);
}

std::ostream& operator<<(std::ostream& os, const Float3& a)
{
	os << "(" << a[0] << ", " << a[1] << ", " << a[2] << ")";
	return os;
}