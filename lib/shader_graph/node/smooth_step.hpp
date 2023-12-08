#pragma once
#include "node.hpp"
namespace shadergraph {
	class SmoothStepNode : public Node
	{
	public:
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;
		void add_io(IOContext& io_ctx) override;

	private:
		size_t m_input_a;
		size_t m_input_b;
		size_t m_input_c;
		size_t m_output;
	};
}   // namespace shadergraph