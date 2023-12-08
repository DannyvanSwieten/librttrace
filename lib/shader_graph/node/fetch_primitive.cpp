#include "fetch_primitive.hpp"

namespace shadergraph {
	void FetchPrimitiveNode::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		const auto& buffer_index = io_ctx.input_value(m_buffer_index);
		io_ctx.set_instruction(m_v0_output, LoadAttribute{ buffer_index, 0 }, ctx);
		io_ctx.set_instruction(m_v1_output, LoadAttribute{ buffer_index, 1 }, ctx);
		io_ctx.set_instruction(m_v2_output, LoadAttribute{ buffer_index, 2 }, ctx);
	}

	void FetchPrimitiveNode::add_io(IOContext& io_ctx)
	{
		m_buffer_index = io_ctx.add_input({ "BufferIndex", "buffer_index" }, Float3(0));

		m_v0_output = io_ctx.add_output({ "V0", "v0" });
		m_v1_output = io_ctx.add_output({ "V1", "v1" });
		m_v2_output = io_ctx.add_output({ "V2", "v2" });
	}
}   // namespace shadergraph