#include "inputs.hpp"
namespace shadergraph {
	void ThreadIdInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::THREAD_ID });
	}

	void ThreadIdInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Thread ID", "thread_id" });
	}
	void ThreadCountInput::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		io_ctx.set_instruction(m_output_index, instructions::Load{ vm::globals::THREAD_COUNT });
	}
	void ThreadCountInput::add_io(IOContext& io_ctx)
	{
		io_ctx.add_output({ "Thread Count", "thread_count" });
	}
}   // namespace shadergraph