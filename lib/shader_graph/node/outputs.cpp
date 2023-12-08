#include "outputs.hpp"

namespace shadergraph {
	void RayOriginOutput::add_io(IOContext& io_ctx)
	{
		m_input_index = io_ctx.add_input({ "Ray Origin", "ray_origin" }, Float3(0.0f, 0.0f, 0.0f));
		m_output_index = io_ctx.add_output({ "Ray Origin", "ray_origin" });
	}

	void RayOriginOutput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		auto output = Store{ io_ctx.input_value(m_input_index), Register{ 0 } };
		io_ctx.set_instruction(m_output_index, output, ctx);
	}
	void RayDirectionOutput::add_io(IOContext& io_ctx)
	{
		m_input_index = io_ctx.add_input({ "Ray Direction", "ray_dir" }, Float3(0.0f, 0.0f, 0.0f));
		m_output_index = io_ctx.add_output({ "Ray Direction", "ray_dir" });
	}
	void RayDirectionOutput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		auto output = Store{ io_ctx.input_value(m_input_index), Register{ 1 } };
		io_ctx.set_instruction(m_output_index, output, ctx);
	}
}   // namespace shadergraph