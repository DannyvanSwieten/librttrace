#pragma once
#include "node.hpp"
namespace shadergraph {

	class FetchPrimitiveNode : public Node
	{
	public:
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;
		void add_io(IOContext& io_ctx) override;

	private:
		size_t m_buffer_index;

		size_t m_v0_output;
		size_t m_v1_output;
		size_t m_v2_output;
	};
}   // namespace shadergraph