#pragma once
#include "api.hpp"
#include "result.hpp"
#include "shader_graph/instructions.hpp"

class RayGenerationProgram;
class ClosestHitProgram;
class MissProgram;

class ShaderCompiler
{
public:
	ShaderCompiler(Api api);
	Result<RayGenerationProgram*> compile_ray_generation_program(const std::vector<shadergraph::Instruction>& program);
	Result<ClosestHitProgram*> compile_closest_hit_program(const std::vector<shadergraph::Instruction>& program);
	Result<MissProgram*> compile_miss_program(const std::vector<shadergraph::Instruction>& program);

private:
	Api m_api;
};