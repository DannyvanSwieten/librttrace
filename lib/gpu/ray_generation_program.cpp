#include "ray_generation_program.hpp"
#include "../overloaded.hpp"
#include "../shader_graph/registers.hpp"
#include "../shader_graph/value.hpp"
#include "../shader_graph/type_deduction.hpp"
#include <sstream>
#include <iostream>

std::string get_global_name(const Global& global)
{
	switch (global.name)
	{
		case globals::THREAD_ID.name:
			return "gl_LaunchIDEXT.xy";
		case globals::THREAD_COUNT.name:
			return "gl_LaunchSizeEXT.xy";
		case globals::INSTANCE_ID.name:
			return "gl_InstanceCustomIndexEXT";
		case globals::PRIMITIVE_ID.name:
			return "gl_PrimitiveID";
	}

	return "";
}

std::string get_register_name(const Register& reg)
{
	return "register_" + std::to_string(reg.index);
}

std::string to_string(const shadergraph::Value& value)
{
	return std::visit(overloaded{ [](int i) {
									 return std::to_string(i);
								 },
	                              [](float f) {
									  return std::to_string(f);
								  },
	                              [](const Float2& f2) {
									  return "vec2(" + std::to_string(f2.x) + ", " + std::to_string(f2.y) + ")";
								  },
	                              [](const Float3& f3) {
									  return "vec3(" + std::to_string(f3.x) + ", " + std::to_string(f3.y) + ", " + std::to_string(f3.z) + ")";
								  },
	                              [](const Float4& f4) {
									  return "vec4(" + std::to_string(f4.x) + ", " + std::to_string(f4.y) + ", " + std::to_string(f4.z) + ", " +
		                                     std::to_string(f4.w) + ")";
								  } },
	                  value);
}

std::string get_operand_name(const shadergraph::Operand& operand)
{
	return std::visit(overloaded{ [](const shadergraph::Value& value) {
									 return to_string(value);
								 },
	                              [](const Register& reg) {
									  return get_register_name(reg);
								  },
	                              [](const Global& global) {
									  return get_global_name(global);
								  } },
	                  operand);
}

std::string get_destination_type(const shadergraph::Instruction& instruction)
{
	return to_string(deduce_output_type(instruction.op_code));
}

GpuRayGenerationProgramGlsl::GpuRayGenerationProgramGlsl(const std::vector<shadergraph::Instruction>& program)
{
	std::stringstream ss;
	ss << "#version 460" << std::endl;
	ss << "void main()" << std::endl;
	ss << "{" << std::endl;

	for (const auto& instruction : program)
	{
		const auto dst_type = get_destination_type(instruction);
		std::visit(overloaded{ [&](const shadergraph::Add& add) {
								  ss << dst_type << " " << get_register_name(instruction.dst) << " = " << get_operand_name(add.a) << " + "
									 << get_operand_name(add.b) << ";" << std::endl;
							  },
		                       [&](const shadergraph::Sub& sub) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = " << get_operand_name(sub.a) << " - "
									  << get_operand_name(sub.b) << ";" << std::endl;
							   },
		                       [&](const shadergraph::Mul& mul) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = " << get_operand_name(mul.a) << " * "
									  << get_operand_name(mul.b) << ";" << std::endl;
							   },
		                       [&](const shadergraph::Div& div) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = " << get_operand_name(div.a) << " / "
									  << get_operand_name(div.b) << ";" << std::endl;
							   },
		                       [&](const shadergraph::Dot& dot) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = dot(" << get_operand_name(dot.a) << ", "
									  << get_operand_name(dot.b) << ");" << std::endl;
							   },
		                       [&](const shadergraph::Cross& cross) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = cross(" << get_operand_name(cross.a) << ", "
									  << get_operand_name(cross.b) << ");" << std::endl;
							   },
		                       [&](const shadergraph::Mix& mix) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = mix(" << get_operand_name(mix.a) << ", "
									  << get_operand_name(mix.b) << ", " << get_operand_name(mix.c) << ");" << std::endl;
							   },
		                       [&](const shadergraph::SmoothStep& smooth_step) {

							   },
		                       [&](const shadergraph::Neg& neg) {

							   },
		                       [&](const shadergraph::Load& load) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = " << get_operand_name(load.src) << ";" << std::endl;
							   },
		                       [&](const shadergraph::LoadAttribute& load_att) {

							   },
		                       [&](const shadergraph::Store& store) {

							   },
		                       [&](const shadergraph::StorePixel& store_pixel) {

							   },
		                       [&](const shadergraph::Normalize& norm) {
								   ss << dst_type << " " << get_register_name(instruction.dst) << " = normalize(" << get_operand_name(norm.src) << ");"
									  << std::endl;
							   },
		                       [&](const shadergraph::Length& length) {

							   },
		                       [&](const shadergraph::Abs& abs) {

							   },
		                       [&](const shadergraph::Sin& sin) {

							   },
		                       [&](const shadergraph::Cos& cos) {

							   },
		                       [&](const shadergraph::Fract& fract) {

							   },
		                       [&](const shadergraph::Intersect& intersect) {
								   ss << "traceRayEXT(" << get_operand_name(intersect.acc_index) << ", " << 0xff << ", 0, 0, 0, "
									  << get_operand_name(intersect.origin) << ", t_min, " << get_operand_name(intersect.direction) << ", t_max, " << 0 << ");"
									  << std::endl;
							   } },
		           instruction.op_code);
	}

	ss << "}" << std::endl;
	m_glsl_code = ss.str();

	std::cout << m_glsl_code << std::endl;
}