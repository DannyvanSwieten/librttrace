#include "intersect.hpp"

namespace shadergraph {

	void IntersectNode::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		const auto& origin = io_ctx.input_value(m_origin_id);
		const auto& direction = io_ctx.input_value(m_direction_id);
		const auto& index = io_ctx.input_value(m_index_id);

		io_ctx.set_instruction(m_output_index, Intersect{ .origin = origin, .direction = direction, .acc_index = index }, ctx);
	}
	void IntersectNode::add_io(IOContext& io_ctx)
	{
		m_origin_id = io_ctx.add_input({ "Origin", "origin" }, Float3(0.0f, 0.0f, 0.0f));
		m_direction_id = io_ctx.add_input({ "Direction", "direction" }, Float3(0.0f, 0.0f, 0.0f));
		m_index_id = io_ctx.add_input({ "Index", "index" }, Float3(0.0f, 0.0f, 0.0f));

		m_output_index = io_ctx.add_output({ "Intersection", "intersection" });
	}
}   // namespace shadergraph