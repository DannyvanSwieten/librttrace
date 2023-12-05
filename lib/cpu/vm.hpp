#pragma once
#include <array>
#include "../compiler/instructions.hpp"

#include "globals.hpp"
class ResourceContext;
class Pipeline;
class VirtualMachine
{
public:
	void execute(const Pipeline* pipeline,
	             ResourceContext* ctx,
	             vm::globals::Globals& globals,
	             int thread_id_x,
	             int thread_id_y,
	             int thread_count_x,
	             int thread_count_y);
	void execute(const instructions::ShaderProgram& program,
	             const Pipeline* pipeline,
	             ResourceContext* ctx,
	             vm::globals::Globals& globals,
	             int thread_id_x,
	             int thread_id_y,
	             int thread_count_x,
	             int thread_count_y);

	template <typename T>
	void execute(const T* const stage,
	             const Pipeline* pipeline,
	             ResourceContext* ctx,
	             vm::globals::Globals& globals,
	             int thread_id_x,
	             int thread_id_y,
	             int thread_count_x,
	             int thread_count_y)
	{
		execute(stage->program(), pipeline, ctx, globals, thread_id_x, thread_id_y, thread_count_x, thread_count_y);
	}

private:
	std::array<Float3, 1024> registers;
};