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
		return { Overload().with_args(0).with_return(Type::Float),
			     Overload().with_args(1).with_return(Type::Float2),
			     Overload().with_args(2).with_return(Type::Float3),
			     Overload().with_args(3).with_return(Type::Float4) };
	}
	Node* ConstantNodeConstructor::construct(const Overload& overload) const
	{
		return nullptr;
	}
}   // namespace shadergraph