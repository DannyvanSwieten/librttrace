#pragma once
#include <variant>
#include "register.hpp"
#include "globals.hpp"
#include "registers.hpp"

namespace shadergraph {
	using Value = std::variant<int, float, Float2, Float3, Float4>;
	using Operand = std::variant<Value, Register, Global>;

	Float4 get_value(const Operand& value, const Registers& registers, globals::Globals& globals);
	Float4 get_value(const Value& constant);
}   // namespace shadergraph