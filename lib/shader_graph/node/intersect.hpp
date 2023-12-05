#pragma once
#include "node.hpp"

namespace shadergraph {
	class Intersect : public Node
	{
	public:
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;
		void add_io(IOContext& io_ctx) override;

	private:
		size_t m_origin_id;
		size_t m_direction_id;
		size_t m_index_id;

		size_t m_output_index;
	};
}   // namespace shadergraph