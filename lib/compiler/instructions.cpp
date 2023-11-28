#include "instructions.hpp"
#include <array>
#include <cassert>
Float3 get_value(const Value& value, const std::array<Float3, 16>& registers, const std::array<Value, 4>& globals)
{
	return std::visit(
		[&](auto&& arg) -> Float3 {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, Float3>)
			{
				return arg;
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				return Float3(arg, arg, arg);
			}
			else if constexpr (std::is_same_v<T, int>)
			{
				return Float3(static_cast<float>(arg), static_cast<float>(arg), static_cast<float>(arg));
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
				return Float3(0.0f, 0.0f, 0.0f);
			}
		},
		value);
}