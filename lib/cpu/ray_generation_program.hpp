#pragma once
#include <array>
#include "../ray_generation_program.hpp"
#include "virtual_machine/shader_program.hpp"

class CpuRayGenerationProgram : public RayGenerationProgram
{
public:
	CpuRayGenerationProgram(const ShaderProgram& program);

	const ShaderProgram& program() const
	{
		return m_program;
	}

private:
	ShaderProgram m_program;
};