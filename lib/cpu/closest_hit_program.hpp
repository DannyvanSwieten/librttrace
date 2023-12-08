#pragma once

#include "../shader_graph/instructions.hpp"
#include "../closest_hit_program.hpp"
class CpuClosestHitProgram : public ClosestHitProgram
{
public:
	CpuClosestHitProgram(const std::vector<shadergraph::Instruction>& program);

	const std::vector<shadergraph::Instruction>& program() const
	{
		return m_program;
	}

private:
	std::vector<shadergraph::Instruction> m_program;
};