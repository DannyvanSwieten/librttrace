#pragma once
#include "value.hpp"
#include "../overloaded.hpp"
#include "instructions.hpp"
Type output_type(const shadergraph::Operand& operand)
{
	return std::visit(overloaded{ [](const shadergraph::Value& value) {
									 return std::visit(overloaded{ [](int i) {
																	  return Type::Int;
																  },
		                                                           [](float f) {
																	   return Type::Float;
																   },
		                                                           [](const Float2& f2) {
																	   return Type::Float2;
																   },
		                                                           [](const Float3& f3) {
																	   return Type::Float3;
																   },
		                                                           [](const Float4& f4) {
																	   return Type::Float4;
																   } },
		                                               value);
								 },
	                              [](const Register& reg) {
									  return reg.type;
								  },
	                              [](const Global& global) {
									  return global.type;
								  } },
	                  operand);
}

Type deduce_output_type(const shadergraph::OpCode opcode)
{
	return std::visit(overloaded{ [&](const shadergraph::Add& add) {
									 return output_type(add.a);
								 },
	                              [&](const shadergraph::Sub& sub) {
									  return output_type(sub.a);
								  },
	                              [&](const shadergraph::Mul& mul) {
									  return output_type(mul.a);
								  },
	                              [&](const shadergraph::Div& div) {
									  return output_type(div.a);
								  },
	                              [&](const shadergraph::Dot& dot) {
									  return Type::Float;
								  },
	                              [&](const shadergraph::Cross& cross) {
									  return Type::Float3;
								  },
	                              [&](const shadergraph::Mix& mix) {
									  return output_type(mix.a);
								  },
	                              [&](const shadergraph::SmoothStep& smooth_step) {
									  return Type::Float;
								  },
	                              [&](const shadergraph::Neg& neg) {
									  return output_type(neg.a);
								  },
	                              [&](const shadergraph::Load& load) {
									  return output_type(load.src);
								  },
	                              [&](const shadergraph::LoadAttribute& load_att) {
									  return Type::Float4;
								  },
	                              [&](const shadergraph::Store& store) {
									  return output_type(store.src);
								  },
	                              [&](const shadergraph::StorePixel& store_pixel) {
									  return Type::Void;
								  },
	                              [&](const shadergraph::Normalize& norm) {
									  return output_type(norm.src);
								  },
	                              [&](const shadergraph::Length& length) {
									  return Type::Float;
								  },
	                              [&](const shadergraph::Abs& abs) {
									  return output_type(abs.a);
								  },
	                              [&](const shadergraph::Sin& sin) {
									  return Type::Float;
								  },
	                              [&](const shadergraph::Cos& cos) {
									  return Type::Float;
								  },
	                              [&](const shadergraph::Fract& fract) {
									  return output_type(fract.a);
								  },
	                              [&](const shadergraph::Intersect& intersect) {
									  return Type::Void;
								  } },
	                  opcode);
}