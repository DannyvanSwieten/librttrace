#pragma once
#include "../context.hpp"
#include "../../compiler/instructions.hpp"
#include "../../cpu/vm.hpp"
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