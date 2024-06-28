#include "constant.hpp"

namespace shadergraph {
	void ConstantNode::output_instruction(CompilerContext& ctx, IOContext& io_ctx)
	{
		auto instruction = Load{ io_ctx.input_value(m_input_index) };
		io_ctx.set_instruction(m_output_index, instruction, ctx);
	}
	void ConstantNode::add_io(IOContext& io_ctx)
	{
		m_input_index = io_ctx.add_input({ "Value", "value" }, Float3(1.0f));
		m_output_index = io_ctx.add_output({ "Result", "result" });
	}
	std::vector<Overload> ConstantNodeConstructor::overloads() const
	{
		return { Overload{ {}, Type::Float }, Overload{ {}, Type::Float2 }, Overload{ {}, Type::Float3 }, Overload{ {}, Type::Float4 } };
	}
	Node* ConstantNodeConstructor::construct(const Overload& overload) const
	{
		return nullptr;
	}
}   // namespace shadergraph