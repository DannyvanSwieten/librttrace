#pragma once
#include "node.hpp"
namespace shadergraph {
	class StorePixel : public Node
	{
	public:
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;
		void add_io(IOContext& io_ctx) override;

	private:
		size_t m_image_index_id;
		size_t m_location_id;
		size_t m_color_id;

		size_t m_output_index;
	};

}   // namespace shadergraph