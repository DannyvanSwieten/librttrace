#pragma once

class RayGenerationProgram;
class MissProgram;
class HitGroup;
class Pipeline
{
public:
	Pipeline(const RayGenerationProgram* ray_generation_program, const MissProgram* miss_program, const HitGroup* hit_groups, size_t hit_group_count);

	const RayGenerationProgram* ray_generation_program() const
	{
		return m_ray_generation_program;
	}

private:
	const RayGenerationProgram* m_ray_generation_program;
	const MissProgram* m_miss_program;
	const HitGroup* m_hit_groups;
	size_t m_hit_group_count;
};