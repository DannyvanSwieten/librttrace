#include "constant.hpp"

namespace shadergraph {
	void ConstantNode::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		auto output = Load{ io_ctx.input_value(m_input_index) };
		io_ctx.set_instruction(m_output_index, output, ctx);
	}
	void ConstantNode::add_io(IOContext& io_ctx)
	{
		m_input_index = io_ctx.add_input({ "Value", "value" }, Float3(1.0f));
		m_output_index = io_ctx.add_output({ "Result", "result" });
	}
}   // namespace shadergraph