#include "store_pixel.hpp"
#include "../../compiler/instructions.hpp"
namespace shadergraph {
	void StorePixel::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		auto image_index = io_ctx.input_value(m_image_index_id);
		auto location = io_ctx.input_value(m_location_id);
		auto color = io_ctx.input_value(m_color_id);

		io_ctx.set_instruction(m_output_index, { instructions::StorePixel{ image_index, location, color } }, ctx);
	}

	void StorePixel::add_io(IOContext& io_ctx)
	{
		m_image_index_id = io_ctx.add_input({ "image_index", "Image Index" }, Float3(0));
		m_location_id = io_ctx.add_input({ "location", "Location" }, Float3(0));
		m_color_id = io_ctx.add_input({ "color", "Color" }, Float3(0));

		m_output_index = io_ctx.add_output({ "out", "Out" });
	}
}   // namespace shadergraph