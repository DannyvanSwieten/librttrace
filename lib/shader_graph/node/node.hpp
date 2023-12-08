#pragma once
#include "../context.hpp"
#include "../../shader_graph/instructions.hpp"
#include "../../cpu/virtual_machine/vm.hpp"
namespace shadergraph {
	class CompilerContext;
	class IOContext;
	class Node
	{
	public:
		virtual ~Node() = 0;
		virtual void output_instruction(CompilerContext& ctx, IOContext& io_ctx) = 0;
		virtual void add_io(IOContext& io_ctx) = 0;
	};

}   // namespace shadergraph