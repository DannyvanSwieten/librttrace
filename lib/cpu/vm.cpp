#include "vm.hpp"
#include <array>
#include <iostream>

template <class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;
};

const std::array<Float3, 16>& VirtualMachine::execute(
	const instructions::ShaderProgram& program, std::array<Value, 4>& globals, int thread_id_x, int thread_id_y, int thread_count_x, int thread_count_y)
{
	globals[vm::globals::THREAD_ID.name] = Float3{ static_cast<float>(thread_id_x), static_cast<float>(thread_id_y), 0.0f };
	globals[vm::globals::THREAD_COUNT.name] = Float3{ static_cast<float>(thread_count_x), static_cast<float>(thread_count_y), 1.0f };
	for (const auto& instruction : program.instructions)
	{
		std::visit(overload{
					   [&](const instructions::Add& add) {
						   const auto a = get_value(add.a, registers, globals);
						   const auto b = get_value(add.b, registers, globals);
						   const auto result = a + b;
						   // std::cout << "Op Add: r" << instruction.dst.index << " = " << a << " + " << b << std::endl;
						   registers[instruction.dst.index] = result;
						   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
					   },
					   [&](const instructions::Sub& sub) {
						   const auto a = get_value(sub.a, registers, globals);
						   const auto b = get_value(sub.b, registers, globals);
						   const auto result = a - b;
						   // std::cout << "Op Sub: r" << instruction.dst.index << " = " << a << " - " << b << std::endl;
						   registers[instruction.dst.index] = result;
						   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
					   },
					   [&](const instructions::Mul& mul) {
						   const auto a = get_value(mul.a, registers, globals);
						   const auto b = get_value(mul.b, registers, globals);
						   const auto result = a * b;
						   // std::cout << "Op Mul: r" << instruction.dst.index << " = " << a << " * " << b << std::endl;
						   registers[instruction.dst.index] = result;
						   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
					   },
					   [&](const instructions::Div& div) {
						   const auto a = get_value(div.a, registers, globals);
						   const auto b = get_value(div.b, registers, globals);
						   const auto result = a / b;
						   // std::cout << "Op Div: r" << instruction.dst.index << " = " << a << " / " << b << std::endl;
						   registers[instruction.dst.index] = result;
						   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
					   },
					   [&](const instructions::Neg& neg) {
						   // std::cout << "Neg" << std::endl;
					   },
					   [&](const instructions::Load& load) {
						   const auto src = get_value(load.src, registers, globals);
						   // std::cout << "Op Load: r" << instruction.dst.index << " with " << src << std::endl;
						   registers[instruction.dst.index] = src;
						   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
					   },
					   [&](const instructions::Store& store) {
						   const auto src = get_value(store.src, registers, globals);
						   // std::cout << "Op Store: " << get_value(store.src, registers, globals) << " -> " << store.name.index << std::endl;
						   registers[store.name.index] = src;
						   // std::cout << "r " << store.name.index << " == " << get_value(registers[store.name.index], registers, globals) << std::endl;
					   },
					   [&](const instructions::Call& call) {
						   // std::cout << "Call" << std::endl;
					   },
					   [&](const instructions::Return& ret) {
						   // std::cout << "Return" << std::endl;
					   },
					   [&](const instructions::Normalize& norm) {
						   const auto src = get_value(norm.src, registers, globals);
						   const auto result = normalize(src);
						   // std::cout << "Op Normalize: r" << instruction.dst.index << " = "
			               //    << "normalize( " << src << " )" << std::endl;
						   registers[instruction.dst.index] = result;
						   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
					   },
				   },
		           instruction.op_code);
	}

	return registers;
}