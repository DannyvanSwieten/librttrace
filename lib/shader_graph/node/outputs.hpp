#pragma once
#include "node.hpp"
namespace shadergraph {
	class RayOriginOutput : public Node
	{
	public:
		void add_io(IOContext& io_ctx) override;
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;

	private:
		size_t m_input_index;
		size_t m_output_index;
	};

	class RayDirectionOutput : public Node
	{
	public:
		void add_io(IOContext& io_ctx) override;
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;

	private:
		size_t m_input_index;
		size_t m_output_index;
	};
}   // namespace shadergraph