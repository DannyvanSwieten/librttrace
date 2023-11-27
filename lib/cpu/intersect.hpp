#pragma once

#include "../bounding_box.hpp"

struct HitRecord
{
	float t = std::numeric_limits<float>::max();
	float u = 0;
	float v = 0;
	uint32_t instance_id = -1;
	uint32_t primitive_id = -1;
};

bool intersect_muller_trumbore(
	const Float3& ray_origin, const Float3& ray_direction, const Float3& v0, const Float3& v1, const Float3& v2, float& t, float& u, float& v);

bool intersect_bounding_box(const Float3& ray_origin, const Float3& ray_direction, const BoundingBox& bounding_box, float& t_min, float t);