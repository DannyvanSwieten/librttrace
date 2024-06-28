#pragma once
#include <vector>
#include "../ray_generation_program.hpp"
#include "../shader_graph/instructions.hpp"
class GpuRayGenerationProgramGlsl : public RayGenerationProgram
{
public:
	GpuRayGenerationProgramGlsl(const std::vector<shadergraph::Instruction>& program);

	const std::string_view glsl_code() const
	{
		return m_glsl_code;
	}

private:
	std::string m_glsl_code;
};