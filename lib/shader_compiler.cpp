#include "shader_compiler.hpp"
#include "cpu/ray_generation_program.hpp"
#include "cpu/closest_hit_program.hpp"
ShaderCompiler::ShaderCompiler(Api api) : m_api(api)
{}

Result<RayGenerationProgram*> ShaderCompiler::compile_ray_generation_program(const instructions::ShaderProgram& program)
{
	switch (m_api)
	{
		case Api::CPU:
			return Result<RayGenerationProgram*>::from_value(new CpuRayGenerationProgram(program));
		default:
			return Result<RayGenerationProgram*>::from_error("Unsupported API");
	}
}

Result<ClosestHitProgram*> ShaderCompiler::compile_closest_hit_program(const instructions::ShaderProgram& program)
{
	switch (m_api)
	{
		case Api::CPU:
			return Result<ClosestHitProgram*>::from_value(new CpuClosestHitProgram(program));
		default:
			return Result<ClosestHitProgram*>::from_error("Unsupported API");
	}
}
