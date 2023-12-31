#include "pipeline.hpp"

Pipeline::Pipeline(const RayGenerationProgram* ray_generation_program,
                   const ClosestHitProgram* closest_hit_program,
                   const MissProgram* miss_program,
                   const HitGroup* hit_groups,
                   size_t hit_group_count)
	: m_ray_generation_program(ray_generation_program)
	, m_closest_hit_program(closest_hit_program)
	, m_miss_program(miss_program)
	, m_hit_groups(hit_groups)
	, m_hit_group_count(hit_group_count)
{}