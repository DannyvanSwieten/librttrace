#include "value.hpp"

namespace shadergraph {
	Float4 get_value(const Operand& value, const Registers& registers, globals::Globals& globals)
	{
		return std::visit(
			[&](auto&& arg) -> Float4 {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, Value>)
				{
					return get_value(arg);
				}
				else if constexpr (std::is_same_v<T, Register>)
				{
					return registers.at(arg.index);
				}
				else if constexpr (std::is_same_v<T, Global>)
				{
					return get_value(globals.at(arg.name), registers, globals);
				}
				else
				{
					assert(false);
					return Float4(0.0f, 0.0f, 0.0f);
				}
			},
			value);
	}

	Float4 get_value(const Value& value)
	{
		return std::visit(
			[&](auto&& arg) -> Float4 {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, int>)
				{
					return Float4(static_cast<float>(arg));
				}

				else if constexpr (std::is_same_v<T, float>)
				{
					return Float4(arg);
				}
				else if constexpr (std::is_same_v<T, Float2>)
				{
					return Float4(arg, 0.0f, 0.0f);
				}
				else if constexpr (std::is_same_v<T, Float3>)
				{
					return Float4(arg, 0.0f);
				}
				else if constexpr (std::is_same_v<T, Float4>)
				{
					return arg;
				}
				else
				{
					assert(false);
					return Float4(0.0f, 0.0f, 0.0f, 0.0f);
				}
			},
			value);
	}
}   // namespace shadergraph