#pragma once
#include <vector>
#include "../../shader_graph/value.hpp"
#include "../../shader_graph/instructions.hpp"
struct ShaderProgram
{
	std::vector<shadergraph::Instruction> instructions;

	ShaderProgram(std::vector<shadergraph::Instruction> instructions) : instructions(instructions)
	{}

	ShaderProgram() : instructions()
	{}

	void add_instruction(const shadergraph::Instruction& var)
	{
		instructions.push_back(var);
	}

	const auto& begin() const
	{
		return instructions.begin();
	}

	const auto& end() const
	{
		return instructions.end();
	}
};