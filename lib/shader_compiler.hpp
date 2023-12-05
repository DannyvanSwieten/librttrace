#pragma once
#include "api.hpp"
#include "result.hpp"
#include "compiler/instructions.hpp"

class RayGenerationProgram;
class ClosestHitProgram;
class ShaderCompiler
{
public:
	ShaderCompiler(Api api);
	Result<RayGenerationProgram*> compile_ray_generation_program(const instructions::ShaderProgram& program);
	Result<ClosestHitProgram*> compile_closest_hit_program(const instructions::ShaderProgram& program);

private:
	Api m_api;
};