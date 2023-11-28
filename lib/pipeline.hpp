#pragma once

class RayGenerationProgram;
class MissProgram;
class HitGroup;
class Pipeline
{
public:
	Pipeline(const RayGenerationProgram* const ray_generation_program,
	         const MissProgram* const miss_program,
	         const HitGroup* const hit_groups,
	         size_t hit_group_count);
};