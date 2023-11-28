#include "Float3.hpp"
#include <cmath>

Float3 operator+(const Float3& a, const Float3& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Float3 operator-(const Float3& a, const Float3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Float3 operator*(const Float3& a, const Float3& b)
{
	return { a.x * b.x, a.y * b.y, a.z * b.z };
}

Float3 operator*(const Float3& a, float b)
{
	return { a.x * b, a.y * b, a.z * b };
}

Float3 operator/(const Float3& a, const Float3& b)
{
	return { a.x / b.x, a.y / b.y, a.z / b.z };
}

Float3 operator/(const Float3& a, float b)
{
	return { a.x / b, a.y / b, a.z / b };
}

float dot(const Float3& a, const Float3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Float3 cross(const Float3& a, const Float3& b)
{
	return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
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
	return { std::pow(a.x, b), std::pow(a.y, b), std::pow(a.z, b) };
}

Float3 sqrt(const Float3& a)
{
	return { std::sqrt(a.x), std::sqrt(a.y), std::sqrt(a.z) };
}

Float3 abs(const Float3& a)
{
	return { std::abs(a.x), std::abs(a.y), std::abs(a.z) };
}

Float3 min(const Float3& a, const Float3& b)
{
	return { std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z) };
}

Float3 max(const Float3& a, const Float3& b)
{
	return { std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z) };
}

std::ostream& operator<<(std::ostream& os, const Float3& a)
{
	os << "(" << a.x << ", " << a.y << ", " << a.z << ")";
	return os;
}