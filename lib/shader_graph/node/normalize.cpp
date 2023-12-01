#include "normalize.hpp"

namespace shadergraph {
	void NormalizeNode::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		auto input = io_ctx.input_value(m_input_index);
		auto output = instructions::Normalize{ input };
		io_ctx.set_instruction(m_output_index, output);
	}

	void NormalizeNode::add_io(IOContext& io_ctx)
	{
		m_input_index = io_ctx.add_input({ "Input", "input" }, Float3(1.0f, 1.0f, 1.0f));
		m_output_index = io_ctx.add_output({ "Output", "output" });
	}
}   // namespace shadergraph