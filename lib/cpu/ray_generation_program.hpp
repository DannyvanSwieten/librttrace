#pragma once
#include <array>
#include "../ray_generation_program.hpp"
#include "vm.hpp"

class CpuRayGenerationProgram : public RayGenerationProgram
{
public:
	CpuRayGenerationProgram(const instructions::ShaderProgram& program);

	const instructions::ShaderProgram& program() const
	{
		return m_program;
	}

private:
	instructions::ShaderProgram m_program;
};