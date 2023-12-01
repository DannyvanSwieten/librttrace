#pragma once
// #ifdef __SSE2__
#include <smmintrin.h>
// #endif
#include <ostream>
struct alignas(16) Float3
{
	union f32x4
	{
		float f[4];
		__m128 data;
	};

	f32x4 data;

	Float3() = default;
	Float3(__m128 data) : data({ .data = data })
	{}
	Float3(f32x4 data) : data(data)
	{}
	Float3(float x, float y, float z) : data({ .data = _mm_set_ps(1.0, z, y, x) })
	{}
	Float3(float x) : data({ .data = _mm_set_ps(1.0, x, x, x) })
	{}

	float operator[](int index) const
	{
		return data.f[index];
	}

	Float3 operator+=(const Float3& b)
	{
		data.data = _mm_add_ps(data.data, b.data.data);
		return *this;
	}

	Float3 operator-=(const Float3& b)
	{
		data.data = _mm_sub_ps(data.data, b.data.data);
		return *this;
	}

	Float3 operator*=(const Float3& b)
	{
		data.data = _mm_mul_ps(data.data, b.data.data);
		return *this;
	}

	Float3 operator*=(float b)
	{
		data.data = _mm_mul_ps(data.data, _mm_set_ps(b, b, b, 0.0));
		return *this;
	}

	Float3 operator/=(const Float3& b)
	{
		data.data = _mm_div_ps(data.data, b.data.data);
		return *this;
	}

	Float3 operator/=(float b)
	{
		data.data = _mm_div_ps(data.data, _mm_set_ps(b, b, b, 1.0));
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