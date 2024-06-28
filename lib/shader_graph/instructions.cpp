#include "instructions.hpp"
#include "../overloaded.hpp"
namespace shadergraph {
	std::string_view to_string(const OpCode& op_code)
	{
		return std::visit(overloaded{
							  [](const Add&) {
								  return std::string_view("add");
							  },
							  [](const Sub&) {
								  return std::string_view("sub");
							  },
							  [](const Mul&) {
								  return std::string_view("mul");
							  },
							  [](const Div&) {
								  return std::string_view("div");
							  },
							  [](const Dot&) {
								  return std::string_view("dot");
							  },
							  [](const Cross&) {
								  return std::string_view("cross");
							  },
							  [](const Neg&) {
								  return std::string_view("neg");
							  },
							  [](const Load&) {
								  return std::string_view("load");
							  },
							  [](const Store&) {
								  return std::string_view("store");
							  },
							  [](const Normalize&) {
								  return std::string_view("normalize");
							  },
							  [](const LoadAttribute&) {
								  return std::string_view("load_attribute");
							  },
							  [](const StorePixel&) {
								  return std::string_view("store_pixel");
							  },
							  [](const Mix&) {
								  return std::string_view("mix");
							  },
							  [](const SmoothStep&) {
								  return std::string_view("smooth_step");
							  },
							  [](const Length&) {
								  return std::string_view("length");
							  },
							  [](const Abs&) {
								  return std::string_view("abs");
							  },
							  [](const Sin&) {
								  return std::string_view("sin");
							  },
							  [](const Cos&) {
								  return std::string_view("cos");
							  },
							  [](const Fract&) {
								  return std::string_view("fract");
							  },
							  [](const Intersect&) {
								  return std::string_view("intersect");
							  },
						  },
		                  op_code);
	}

	std::string_view to_display_name(const OpCode& op_code)
	{
		return std::visit(overloaded{
							  [](const Add&) {
								  return std::string_view("Add");
							  },
							  [](const Sub&) {
								  return std::string_view("Subtract");
							  },
							  [](const Mul&) {
								  return std::string_view("Multiply");
							  },
							  [](const Div&) {
								  return std::string_view("Divide");
							  },
							  [](const Dot&) {
								  return std::string_view("Dot");
							  },
							  [](const Cross&) {
								  return std::string_view("Cross");
							  },
							  [](const Neg&) {
								  return std::string_view("Negate");
							  },
							  [](const Load&) {
								  return std::string_view("Load");
							  },
							  [](const Store&) {
								  return std::string_view("Store");
							  },
							  [](const Normalize&) {
								  return std::string_view("Normalize");
							  },
							  [](const LoadAttribute&) {
								  return std::string_view("Load Attribute");
							  },
							  [](const StorePixel&) {
								  return std::string_view("Store Pixel");
							  },
							  [](const Mix&) {
								  return std::string_view("Mix");
							  },
							  [](const SmoothStep&) {
								  return std::string_view("Smooth Step");
							  },
							  [](const Length&) {
								  return std::string_view("Length");
							  },
							  [](const Abs&) {
								  return std::string_view("Abs");
							  },
							  [](const Sin&) {
								  return std::string_view("Sin");
							  },
							  [](const Cos&) {
								  return std::string_view("Cos");
							  },
							  [](const Fract&) {
								  return std::string_view("Fract");
							  },
							  [](const Intersect&) {
								  return std::string_view("Intersect");
							  },
						  },
		                  op_code);
	}
}   // namespace shadergraph
