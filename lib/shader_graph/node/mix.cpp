#include "mix.hpp"

namespace shadergraph {
	void MixNode::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		const auto& input_a = io_ctx.input_value(m_input_a);
		const auto& input_b = io_ctx.input_value(m_input_b);
		const auto& input_c = io_ctx.input_value(m_input_c);
		const auto& output = io_ctx.output(m_output);

		io_ctx.set_instruction(m_output, Mix{ input_a, input_b, input_c }, ctx);
	}
	void MixNode::add_io(IOContext& io_ctx)
	{
		m_input_a = io_ctx.add_input({ "a", "a" }, { Float4{ 0.5, 0.7, 1.0, 0.0 } });
		m_input_b = io_ctx.add_input({ "b", "b" }, { Float4{ 1.0, 1.0, 1.0, 1.0 } });
		m_input_c = io_ctx.add_input({ "c", "c" }, { Float4{ 0.0, 0.0, 0.0, 0.0 } });

		m_output = io_ctx.add_output({ "Result", "result" });
	}
}   // namespace shadergraph