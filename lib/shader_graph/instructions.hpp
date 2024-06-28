#pragma once
#include <variant>
#include <vector>
#include <map>
#include <string_view>

#include "../float3.hpp"
#include "value.hpp"

namespace shadergraph {
	struct Add
	{
		Operand a;
		Operand b;
	};

	struct Sub
	{
		Operand a;
		Operand b;
	};

	struct Mul
	{
		Operand a;
		Operand b;
	};

	struct Div
	{
		Operand a;
		Operand b;
	};

	struct Dot
	{
		Operand a;
		Operand b;
	};

	struct Cross
	{
		Operand a;
		Operand b;
	};

	struct Neg
	{
		Operand a;
	};

	struct Load
	{
		Operand src;
	};

	struct Store
	{
		Operand src;
		Register name;
	};

	struct Call
	{
		Operand a;
	};

	struct Return
	{};

	struct Normalize
	{
		Operand src;
	};

	struct Sin
	{
		Operand a;
	};

	struct Cos
	{
		Operand a;
	};

	struct Intersect
	{
		Operand origin;
		Operand direction;
		Operand acc_index;
	};

	struct SmoothStep
	{
		Operand a;
		Operand b;
		Operand c;
	};

	struct StorePixel
	{
		Operand image_index;
		Operand location;
		Operand color;
	};

	struct LoadAttribute
	{
		Operand buffer_index;
		Operand vertex_index;
	};

	struct Mix
	{
		Operand a;
		Operand b;
		Operand c;
	};

	struct Length
	{
		Operand a;
	};

	struct Abs
	{
		Operand a;
	};

	struct Fract
	{
		Operand a;
	};

	using OpCode = std::variant<Add,
	                            Sub,
	                            Mul,
	                            Div,
	                            Dot,
	                            Cross,
	                            Neg,
	                            Mix,
	                            SmoothStep,
	                            Load,
	                            LoadAttribute,
	                            Store,
	                            StorePixel,
	                            Normalize,
	                            Length,
	                            Abs,
	                            Sin,
	                            Cos,
	                            Fract,
	                            Intersect>;

	std::string_view to_string(const OpCode& op_code);
	std::string_view to_display_name(const OpCode& op_code);

	struct Instruction
	{
		OpCode op_code;
		Register dst;
	};
}   // namespace shadergraph
