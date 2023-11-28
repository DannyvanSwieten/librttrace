#include "vm.hpp"
#include <array>
#include <iostream>

template <class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;
};

void VirtualMachine::execute(
	const instructions::ShaderProgram& program, std::array<Value, 4>& globals, int thread_id_x, int thread_id_y, int thread_count_x, int thread_count_y)
{
	std::array<Float3, 16> registers;
	globals[vm::globals::THREAD_ID.name] = Float3{ static_cast<float>(thread_id_x), static_cast<float>(thread_id_y), 0.0f };
	globals[vm::globals::THREAD_COUNT.name] = Float3{ static_cast<float>(thread_count_x), static_cast<float>(thread_count_y), 1.0f };
	for (const auto& instruction : program.instructions)
	{
		std::visit(overload{
					   [&](const instructions::Add& add) {
						   const auto a = get_value(add.a, registers, globals);
						   const auto b = get_value(add.b, registers, globals);
						   const auto result = a + b;
						   // std::cout << "Op Add: r" << add.result.index << " = " << a << " + " << b << std::endl;
						   registers[add.result.index] = result;
						   // std::cout << "r" << add.result.index << " == " << registers[add.result.index] << std::endl;
					   },
					   [&](const instructions::Sub& sub) {
						   const auto a = get_value(sub.a, registers, globals);
						   const auto b = get_value(sub.b, registers, globals);
						   const auto result = a - b;
						   // std::cout << "Op Sub: r" << sub.result.index << " = " << a << " - " << b << std::endl;
						   registers[sub.result.index] = result;
						   // std::cout << "r" << sub.result.index << " == " << registers[sub.result.index] << std::endl;
					   },
					   [&](const instructions::Mul& mul) {
						   const auto a = get_value(mul.a, registers, globals);
						   const auto b = get_value(mul.b, registers, globals);
						   const auto result = a * b;
						   // std::cout << "Op Mul: r" << mul.result.index << " = " << a << " * " << b << std::endl;
						   registers[mul.result.index] = result;
						   // std::cout << "r" << mul.result.index << " == " << registers[mul.result.index] << std::endl;
					   },
					   [&](const instructions::Div& div) {
						   const auto a = get_value(div.a, registers, globals);
						   const auto b = get_value(div.b, registers, globals);
						   const auto result = a / b;
						   // std::cout << "Op Div: r" << div.result.index << " = " << a << " / " << b << std::endl;
						   registers[div.result.index] = result;
						   // std::cout << "r" << div.result.index << " == " << registers[div.result.index] << std::endl;
					   },
					   [&](const instructions::Neg& neg) {
						   // std::cout << "Neg" << std::endl;
					   },
					   [&](const instructions::Load& load) {
						   const auto src = get_value(load.src, registers, globals);
						   // std::cout << "Op Load: r" << load.dst.index << " with " << src << std::endl;
						   registers[load.dst.index] = src;
						   // std::cout << "r" << load.dst.index << " == " << registers[load.dst.index] << std::endl;
					   },
					   [&](const instructions::Store& store) {
						   const auto src = get_value(store.src, registers, globals);
						   // std::cout << "Op Store: " << get_value(store.src, registers, globals) << " -> " << store.name.name << std::endl;
						   globals[store.name.name] = src;
						   // std::cout << "Global " << store.name.name << " == " << get_value(globals[store.name.name], registers, globals) << std::endl;
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
						   // std::cout << "Op Normalize: r" << norm.dst.index << " = "
			               //    << "normalize( " << src << " )" << std::endl;
						   registers[norm.dst.index] = result;
						   // std::cout << "r" << norm.dst.index << " == " << registers[norm.dst.index] << std::endl;
					   },
				   },
		           instruction);
	}
}