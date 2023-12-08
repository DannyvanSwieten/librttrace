#pragma once
#include <map>
#include "instructions.hpp"
namespace shadergraph {
	class CompilerContext
	{
	public:
		uint32_t next_free_register();
		uint32_t m_next_free_register = 0;
	};

	struct PortDescription
	{
		std::string_view name;
		std::string_view id;
	};

	struct Input
	{
		Operand Operand;
	};

	struct Output
	{
		Instruction instruction;
	};

	class IOContext
	{
	public:
		size_t add_input(const PortDescription& description, const Operand& default_Operand);
		size_t add_output(const PortDescription& output_description);

		const Operand& input_value(size_t index) const;
		const Instruction& output(size_t index) const;

		void set_input_value(size_t index, const Operand& Operand);
		void set_instruction(size_t index, const OpCode& op_code, CompilerContext& compiler_ctx);

		const PortDescription& input_description(size_t index) const;
		const PortDescription& output_description(size_t index) const;

		const std::map<size_t, Output>& output_values() const
		{
			return m_output_value;
		}

	private:
		std::map<size_t, Input> m_input_value;
		std::map<size_t, Output> m_output_value;

		std::map<size_t, PortDescription> m_input_descriptions;
		std::map<size_t, PortDescription> m_output_descriptions;

		size_t m_input_id = 0;
		size_t m_output_id = 0;
	};
}   // namespace shadergraph