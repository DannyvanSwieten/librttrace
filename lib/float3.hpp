#pragma once
#include <ostream>
struct alignas(16) Float3
{
	float x, y, z;

	Float3() = default;
	Float3(float x, float y, float z) : x(x), y(y), z(z)
	{}
	Float3(float x) : x(x), y(x), z(x)
	{}

	float operator[](int index) const
	{
		return (&x)[index];
	}

	Float3 operator+=(const Float3& b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return *this;
	}

	Float3 operator-=(const Float3& b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return *this;
	}

	Float3 operator*=(const Float3& b)
	{
		x *= b.x;
		y *= b.y;
		z *= b.z;
		return *this;
	}

	Float3 operator*=(float b)
	{
		x *= b;
		y *= b;
		z *= b;
		return *this;
	}

	Float3 operator/=(const Float3& b)
	{
		x /= b.x;
		y /= b.y;
		z /= b.z;
		return *this;
	}

	Float3 operator/=(float b)
	{
		x /= b;
		y /= b;
		z /= b;
		return *this;
	}
};

Float3 operator+(const Float3& a, const Float3& b);
Float3 operator-(const Float3& a, const Float3& b);
Float3 operator*(const Float3& a, const Float3& b);
Float3 operator*(const Float3& a, float b);
Float3 operator/(const Float3& a, const Float3& b);
Float3 operator/(const Float3& a, float b);

float dot(const Float3& a, const Float3& b);
Float3 cross(const Float3& a, const Float3& b);
Float3 normalize(const Float3& a);
Float3 reflect(const Float3& a, const Float3& b);
Float3 refract(const Float3& a, const Float3& b, float eta);
Float3 pow(const Float3& a, float b);
Float3 sqrt(const Float3& a);
Float3 abs(const Float3& a);
Float3 min(const Float3& a, const Float3& b);
Float3 max(const Float3& a, const Float3& b);

std::ostream& operator<<(std::ostream& os, const Float3& a);