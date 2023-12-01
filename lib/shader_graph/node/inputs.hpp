#pragma once
#include "node.hpp"
namespace shadergraph {
	class ThreadIdInput : public Node
	{
	public:
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;
		void add_io(IOContext& io_ctx) override;

	private:
		size_t m_output_index;
	};

	class ThreadCountInput : public Node
	{
	public:
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;
		void add_io(IOContext& io_ctx) override;

	private:
		size_t m_output_index;
	};
}   // namespace shadergraph