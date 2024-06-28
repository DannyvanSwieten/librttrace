#include "constructor.hpp"
#include "../overload.hpp"
#include "../context.hpp"

namespace shadergraph {
	void Constructor::add_io(IOContext& ctx, const Overload& overload) const
	{
		size_t input_index = 0;
		for (const auto& arg : overload.args())
		{
			const auto name = input_name(overload, input_index);
			const auto default_value = input_default(overload, input_index);
			ctx.add_input({ name, name }, default_value);
		}
	}
}   // namespace shadergraph