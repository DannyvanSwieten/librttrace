#include "context.hpp"
#include <cassert>
namespace shadergraph {
	uint32_t CompilerContext::next_free_register()
	{
		return m_next_free_register++;
	}
	size_t IOContext::add_input(const PortDescription& description, const Operand& default_Operand)
	{
		size_t index = m_input_id++;
		m_input_descriptions.emplace(index, description);
		m_input_value.emplace(index, Input{ default_Operand });
		return index;
	}
	size_t IOContext::add_output(const PortDescription& output_description)
	{
		size_t index = m_output_id++;
		m_output_descriptions.emplace(index, output_description);
		m_output_value.emplace(index, Output{});
		return index;
	}
	const Operand& IOContext::input_value(size_t index) const
	{
		auto it = m_input_value.find(index);
		if (it != m_input_value.end())
		{
			return it->second.Operand;
		}
		assert(false);
		return {};
	}
	const Instruction& IOContext::output(size_t index) const
	{
		auto it = m_output_value.find(index);
		if (it != m_output_value.end())
		{
			return it->second.instruction;
		}
		assert(false);
		return {};
	}
	void IOContext::set_input_value(size_t index, const Operand& Operand)
	{
		auto it = m_input_value.find(index);
		if (it != m_input_value.end())
		{
			it->second.Operand = Operand;
		}
		else
		{
			assert(false);
		}
	}
	void IOContext::set_instruction(size_t index, const OpCode& op_code, CompilerContext& compiler_ctx)
	{
		const auto instruction = Instruction{ op_code, Register{ compiler_ctx.next_free_register() } };
		auto it = m_output_value.find(index);
		if (it != m_output_value.end())
		{
			it->second.instruction = std::move(instruction);
		}
		else
		{
			assert(false);
		}
	}
}   // namespace shadergraph