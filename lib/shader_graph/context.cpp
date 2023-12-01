#include "context.hpp"
#include <cassert>
namespace shadergraph {
	uint32_t CompilerContext::next_free_register()
	{
		return m_next_free_register++;
	}
	size_t IOContext::add_input(const PortDescription& description, const Value& default_value)
	{
		size_t index = m_id++;
		m_input_descriptions.emplace(index, description);
		m_input_value.emplace(index, Input{ default_value });
		return index;
	}
	size_t IOContext::add_output(const PortDescription& output_description)
	{
		size_t index = m_id++;
		m_output_descriptions.emplace(index, output_description);
		m_output_value.emplace(index, Output{});
		return index;
	}
	const Value& IOContext::input_value(size_t index) const
	{
		auto it = m_input_value.find(index);
		if (it != m_input_value.end())
		{
			return it->second.value;
		}
		assert(false);
		return {};
	}
	const instructions::Instruction& IOContext::output(size_t index) const
	{
		auto it = m_output_value.find(index);
		if (it != m_output_value.end())
		{
			return it->second.instruction;
		}
		assert(false);
		return {};
	}
	void IOContext::set_input_value(size_t index, const Value& value)
	{
		auto it = m_input_value.find(index);
		if (it != m_input_value.end())
		{
			it->second.value = value;
		}
		else
		{
			assert(false);
		}
	}
	void IOContext::set_instruction(size_t index, const instructions::OpCode& op_code)
	{
		const auto instruction = instructions::Instruction{ op_code, Register{ m_compiler_ctx.next_free_register() } };
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