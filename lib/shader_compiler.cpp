#include "shader_compiler.hpp"
#include "cpu/ray_generation_program.hpp"
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
