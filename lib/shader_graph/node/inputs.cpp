#include "inputs.hpp"
namespace shadergraph {
	void ThreadIdInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::THREAD_ID }, ctx);
	}

	void ThreadIdInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Thread ID", "thread_id" });
	}
	void ThreadCountInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::THREAD_COUNT }, ctx);
	}
	void ThreadCountInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Thread Count", "thread_count" });
	}
	void BarycentricCoordinateInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::BARYCENTRIC_COORDINATE }, ctx);
	}
	void BarycentricCoordinateInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Barycentric Coordinate", "barycentric_coordinate" });
	}
	void HitDistanceInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::HIT_DISTANCE }, ctx);
	}
	void HitDistanceInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Hit Distance", "hit_distance" });
	}
	void InstanceIdInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::INSTANCE_ID }, ctx);
	}

	void InstanceIdInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Instance ID", "instance_id" });
	}

	void PrimitiveIdInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::PRIMITIVE_ID }, ctx);
	}

	void PrimitiveIdInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Primitive ID", "primitive_id" });
	}
}   // namespace shadergraph