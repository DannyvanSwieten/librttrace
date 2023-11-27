#include <algorithm>
#include "intersect.hpp"

bool intersect_muller_trumbore(
	const Float3& ray_origin, const Float3& ray_direction, const Float3& v0, const Float3& v1, const Float3& v2, float& t, float& u, float& v)
{
	const auto edge1 = v1 - v0;
	const auto edge2 = v2 - v0;
	const auto pvec = cross(ray_direction, edge2);
	const auto det = dot(edge1, pvec);
	if (det == 0.0f)
	{
		return false;
	}
	const auto inv_det = 1.0f / det;
	const auto tvec = ray_origin - v0;
	u = dot(tvec, pvec) * inv_det;
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}
	const auto qvec = cross(tvec, edge1);
	v = dot(ray_direction, qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}
	t = dot(edge2, qvec) * inv_det;
	return true;
}

bool intersect_bounding_box(const Float3& ray_origin, const Float3& ray_direction, const BoundingBox& bounding_box, float& t_min, float t)
{
	const auto inv_ray_direction = 1.0f / ray_direction;
	const auto t0 = (bounding_box.m_min - ray_origin) * inv_ray_direction;
	const auto t1 = (bounding_box.m_max - ray_origin) * inv_ray_direction;
	const auto t_min_v = min(t0, t1);
	const auto t_max_v = max(t0, t1);
	t_min = std::max(t_min_v.x, std::max(t_min_v.y, t_min_v.z));
	const auto t_max = std::min(t_max_v.x, std::min(t_max_v.y, t_max_v.z));
	return t_max >= std::max(0.0f, t_min) && t_min < t;
}
