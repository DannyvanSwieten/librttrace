#pragma once

#include "node.hpp"

namespace shadergraph {
	template <typename T>
	class BinaryOperatorNode : public Node
	{
	public:
		void add_io(IOContext& io_ctx) override
		{
			m_lhs_input_index = io_ctx.add_input({ "Lhs", "lhs" }, Float3(0.0f, 0.0f, 0.0f));
			m_rhs_input_index = io_ctx.add_input({ "Rhs", "rhs" }, Float3(0.0f, 0.0f, 0.0f));
			m_output_index = io_ctx.add_output({ "Result", "result" });
		}
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override
		{
			auto output = T{ io_ctx.input_value(m_lhs_input_index), io_ctx.input_value(m_rhs_input_index) };
			io_ctx.set_instruction(m_output_index, output, ctx);
		}

	private:
		size_t m_lhs_input_index;
		size_t m_rhs_input_index;
		size_t m_output_index;
	};
}   // namespace shadergraph