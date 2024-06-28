#pragma once
#include <cassert>
#include "node.hpp"
#include "constructor.hpp"
#include "../overload.hpp"
#include "../../overloaded.hpp"

namespace shadergraph {
	template <typename T>
	class BinaryOperatorNode : public Node
	{
	public:
		void add_io(IOContext& io_ctx) override
		{
			m_lhs_input_index = io_ctx.add_input({ "Lhs", "lhs" }, Float3(0.0f, 0.0f, 0.0f));
			m_rhs_input_index = io_ctx.add_input({ "Rhs", "rhs" }, Float3(0.0f, 0.0f, 0.0f));
			m_output_index = io_ctx.add_output({ "Result", "result" });
		}
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override
		{
			auto output = T{ io_ctx.input_value(m_lhs_input_index), io_ctx.input_value(m_rhs_input_index) };
			io_ctx.set_instruction(m_output_index, output, ctx);
		}

	private:
		size_t m_lhs_input_index;
		size_t m_rhs_input_index;
		size_t m_output_index;
	};

	template <typename T>
	class BinaryOperatorConstructor : public Constructor
	{
	public:
		BinaryOperatorConstructor()
		{}

		std::string_view uid() const override
		{
			return to_string(T{});
		}

		std::string_view display_name() const override
		{
			return to_display_name(T{});
		}

		std::vector<Overload> overloads() const override
		{
			if constexpr (std::is_same_v<T, Add> || std::is_same_v<T, Sub> || std::is_same_v<T, Mul> || std::is_same_v<T, Div> || std::is_same_v<T, Dot>)
			{
				return { Overload{ std::array{ Type::Float, Type::Float }, Type::Float },
					     Overload{ std::array{ Type::Float2, Type::Float2 }, Type::Float2 },
					     Overload{ std::array{ Type::Float3, Type::Float3 }, Type::Float3 },
					     Overload{ std::array{ Type::Float4, Type::Float4 }, Type::Float4 } };
			}
			else if constexpr (std::is_same_v<T, Cross>)
			{
				return { Overload{ std::array{ Type::Float3, Type::Float3 }, Type::Float } };
			}
			else
			{
				assert(false && "Unsupported binary operator");
				return {};
			}
		}

		size_t default_overload() const override
		{
			return 0;
		}

		std::string_view input_name(const Overload& overload, size_t input_index) const override
		{
			if (input_index == 0)
			{
				return "lhs";
			}
			else
			{
				return "rhs";
			}
		}

		std::string_view output_name(const Overload& overload) const override
		{
			return "result";
		}

		Operand input_default(const Overload& overload, size_t input_index) const override
		{
			return 0.0f;
		}

		Node* construct(const Overload& overload) const override
		{
			return new BinaryOperatorNode<T>();
		}
	};
	template <typename T>
	class UnaryOperatorNode : public Node
	{
	public:
		void add_io(IOContext& io_ctx) override
		{
			m_input_index = io_ctx.add_input({ "Input", "input" }, Float3(0.0f, 0.0f, 0.0f));
			m_output_index = io_ctx.add_output({ "Result", "result" });
		}
		void output_instruction(CompilerContext& ctx, IOContext& io_ctx) override
		{
			auto output = T{ io_ctx.input_value(m_input_index) };
			io_ctx.set_instruction(m_output_index, output, ctx);
		}

	private:
		size_t m_input_index;
		size_t m_output_index;
	};
}   // namespace shadergraph