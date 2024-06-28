#include "shader_compiler.hpp"
#include "cpu/ray_generation_program.hpp"
#include "cpu/closest_hit_program.hpp"
#include "cpu/miss_program.hpp"
#include "gpu/ray_generation_program.hpp"
ShaderCompiler::ShaderCompiler(Api api) : m_api(api)
{}

Result<RayGenerationProgram*> ShaderCompiler::compile_ray_generation_program(const std::vector<shadergraph::Instruction>& program)
{
	switch (m_api)
	{
		case Api::CPU:
			return Result<RayGenerationProgram*>::from_value(new CpuRayGenerationProgram(program));
		case Api::VULKAN_RTX:
			return Result<RayGenerationProgram*>::from_value(new GpuRayGenerationProgramGlsl(program));
		default:
			return Result<RayGenerationProgram*>::from_error("Unsupported API");
	}
}

Result<ClosestHitProgram*> ShaderCompiler::compile_closest_hit_program(const std::vector<shadergraph::Instruction>& program)
{
	switch (m_api)
	{
		case Api::CPU:
			return Result<ClosestHitProgram*>::from_value(new CpuClosestHitProgram(program));
		default:
			return Result<ClosestHitProgram*>::from_error("Unsupported API");
	}
}

Result<MissProgram*> ShaderCompiler::compile_miss_program(const std::vector<shadergraph::Instruction>& program)
{
	switch (m_api)
	{
		case Api::CPU:
			return Result<MissProgram*>::from_value(new CpuMissProgram(program));
		default:
			return Result<MissProgram*>::from_error("Unsupported API");
	}
}
