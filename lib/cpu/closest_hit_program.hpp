#pragma once

#include "../compiler/instructions.hpp"
#include "../closest_hit_program.hpp"
class CpuClosestHitProgram : public ClosestHitProgram
{
public:
	CpuClosestHitProgram(const instructions::ShaderProgram& program);

	const instructions::ShaderProgram& program() const
	{
		return m_program;
	}

private:
	instructions::ShaderProgram m_program;
};