#pragma once
#include "node.hpp"
#include "constructor.hpp"
#include "../overload.hpp"
namespace shadergraph {
	class ConstantNode : public Node
	{
	public:
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override;
		void add_io(IOContext& io_ctx) override;

	private:
		size_t m_input_index;
		size_t m_output_index;
	};

	class ConstantNodeConstructor : public Constructor
	{
	public:
		std::string_view uid() const override
		{
			return "constant";
		}
		std::string_view display_name() const override
		{
			return "Constant";
		}
		std::vector<Overload> overloads() const;
		size_t default_overload() const override
		{
			return 0;
		}
		Node* construct(const Overload& overload) const;
		std::string_view input_name(const Overload& overload, size_t input_index) const;
		std::string_view output_name(const Overload& overload) const;
		Operand input_default(const Overload& overload, size_t input_index) const;
	};
}   // namespace shadergraph