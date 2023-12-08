#pragma once
#include <vector>
#include "../shader_graph/instructions.hpp"
#include "../miss_program.hpp"

class CpuMissProgram : public MissProgram
{
public:
	CpuMissProgram(const std::vector<shadergraph::Instruction>& program);

	const std::vector<shadergraph::Instruction>& program() const
	{
		return m_program;
	}

private:
	const std::vector<shadergraph::Instruction> m_program;
};