#pragma once
#include <variant>
#include <vector>
#include <map>
#include <string_view>
#include "../float3.hpp"

struct Register
{
	uint32_t index;
};

struct Global
{
	size_t name;
};

using Constant = std::variant<int, float, Float3>;
using Value = std::variant<Constant, Register, Global>;

Float3 get_value(const Value& value, const std::array<Float3, 16>& registers, const std::array<Value, 4>& globals);
Float3 get_value(const Constant& constant);

namespace instructions {
	struct Add
	{
		Value a;
		Value b;
	};

	struct Sub
	{
		Value a;
		Value b;
	};

	struct Mul
	{
		Value a;
		Value b;
	};

	struct Div
	{
		Value a;
		Value b;
	};

	struct Neg
	{
		Value a;
	};

	struct Load
	{
		Value src;
	};

	struct Store
	{
		Value src;
		Register name;
	};

	struct Call
	{
		Value a;
	};

	struct Return
	{};

	struct Normalize
	{
		Value src;
	};

	// struct Jump
	// {
	// 	Value a;
	// };

	// struct JumpIf
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfNot
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfLess
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfLessOrEqual
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfGreater
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfGreaterOrEqual
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfEqual
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfNotEqual
	// {
	// 	Value a;
	// 	Value b;
	// };

	// struct JumpIfZero
	// {
	// 	Value a;
	// };

	// struct JumpIfNotZero
	// {
	// 	Value a;
	// };

	// struct JumpIfNegative
	// {
	// 	Value a;
	// };

	// struct JumpIfNotNegative
	// {
	// 	Value a;
	// };

	// struct JumpIfPositive
	// {
	// 	Value a;
	// };

	// struct JumpIfNotPositive
	// {
	// 	Value a;
	// };

	// struct JumpIfOdd
	// {
	// 	Value a;
	// };

	// struct JumpIfNotOdd
	// {
	// 	Value a;
	// };

	// struct JumpIfEven
	// {
	// 	Value a;
	// };

	// struct JumpIfNotEven
	// {
	// 	Value a;
	// };

	// struct JumpIfCarry
	// {
	// 	Value a;
	// };

	// struct JumpIfNotCarry
	// {
	// 	Value a;
	// };

	// struct JumpIfOverflow
	// {
	// 	Value a;
	// };

	// struct JumpIfNotOverflow
	// {
	// 	Value a;
	// };

	// struct JumpIfSign
	// {
	// 	Value a;
	// };

	// struct JumpIfNotSign
	// {
	// 	Value a;
	// };

	// struct JumpIfZeroOrCarry
	// {
	// 	Value a;
	// };

	// struct JumpIfNotZeroOrCarry
	// {
	// 	Value a;
	// };

	using OpCode = std::variant<Add, Sub, Mul, Div, Neg, Load, Store, Call, Return, Normalize
	                            // Jump,
	                            // JumpIf,
	                            // JumpIfNot,
	                            // JumpIfLess,
	                            // JumpIfLessOrEqual,
	                            // JumpIfGreater,
	                            // JumpIfGreaterOrEqual,
	                            // JumpIfEqual,
	                            // JumpIfNotEqual,
	                            // JumpIfZero,
	                            // JumpIfNotZero,
	                            // JumpIfNegative,
	                            // JumpIfNotNegative,
	                            // JumpIfPositive,
	                            // JumpIfNotPositive,
	                            // JumpIfOdd,
	                            // JumpIfNotOdd,
	                            // JumpIfEven,
	                            // JumpIfNotEven,
	                            // JumpIfCarry,
	                            // JumpIfNotCarry,
	                            // JumpIfOverflow,
	                            // JumpIfNotOverflow,
	                            // JumpIfSign,
	                            // JumpIfNotSign,
	                            // JumpIfZeroOrCarry,
	                            // JumpIfNotZeroOrCarry
	                            >;

	struct Instruction
	{
		OpCode op_code;
		Register dst;
	};

	struct ShaderProgram
	{
		std::vector<Value> variables;
		std::vector<Value> constants;
		std::vector<Instruction> instructions;

		ShaderProgram(std::vector<Value> variables, std::vector<Value> constants, std::vector<Instruction> instructions)
			: variables(variables)
			, constants(constants)
			, instructions(instructions)
		{}

		ShaderProgram() : variables(), constants(), instructions()
		{}

		void add_variable(Value var)
		{
			variables.push_back(var);
		}

		void add_constant(Value var)
		{
			constants.push_back(var);
		}

		void add_instruction(Instruction var)
		{
			instructions.push_back(var);
		}
	};
}   // namespace instructions