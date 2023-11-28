#pragma once
#include <map>
#include "../compiler/instructions.hpp"

namespace vm::globals {
	static constexpr Global THREAD_ID{ 0 };
	static constexpr Global THREAD_COUNT{ 1 };
	static constexpr Global RAY_ORIGIN{ 2 };
	static constexpr Global RAY_DIRECTION{ 3 };

}   // namespace vm::globals

class VirtualMachine
{
public:
	void execute(
		const instructions::ShaderProgram& program, std::array<Value, 4>& globals, int thread_id_x, int thread_id_y, int thread_count_x, int thread_count_y);
};