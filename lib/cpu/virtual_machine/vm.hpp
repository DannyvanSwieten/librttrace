#pragma once
#include <array>
#include "../../shader_graph/registers.hpp"
#include "../../shader_graph/instructions.hpp"
#include "../../shader_graph/globals.hpp"
#include "shader_program.hpp"

class ResourceContext;
class Pipeline;

class VirtualMachine
{
public:
	void execute(
		const Pipeline* pipeline, ResourceContext* ctx, globals::Globals& globals, int thread_id_x, int thread_id_y, int thread_count_x, int thread_count_y);
	void execute(const ShaderProgram& program,
	             const Pipeline* pipeline,
	             ResourceContext* ctx,
	             globals::Globals& globals,
	             int thread_id_x,
	             int thread_id_y,
	             int thread_count_x,
	             int thread_count_y);

	template <typename T>
	void execute(const T* const stage,
	             const Pipeline* pipeline,
	             ResourceContext* ctx,
	             globals::Globals& globals,
	             int thread_id_x,
	             int thread_id_y,
	             int thread_count_x,
	             int thread_count_y)
	{
		execute(stage->program(), pipeline, ctx, globals, thread_id_x, thread_id_y, thread_count_x, thread_count_y);
	}

private:
	Registers registers;
	size_t m_pc = 0;
	std::array<Registers, 3> m_stack;
	size_t m_stack_pointer = 0;
};