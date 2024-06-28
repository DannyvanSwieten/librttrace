#include "document.hpp"
#include "overload.hpp"
#include "constructor_cache.hpp"

namespace shadergraph {
	Document::Document(ConstructorCache& registry) : m_registry(registry)
	{}
	void Document::add_node(size_t node_uid, const std::string& ctor_uid)
	{
		const auto entry = m_registry.get_entry(ctor_uid);
		if (!entry)
			return;

		const auto default_overload = entry->m_ctor->default_overload();
		const auto& overload = entry->m_overloads[default_overload];
		IOContext io_ctx{};
		entry->m_ctor->add_io(io_ctx, overload);
		Slot slot{ ctor_uid, overload, io_ctx };
		m_slots.emplace(node_uid, slot);
	}
	void Document::connect(size_t from_node_uid, size_t from_output, size_t to_node_uid, size_t to_input)
	{
		// Check if the nodes exist
		const auto& entry = m_registry.get_entry(m_slots.at(to_node_uid).m_ctor_name);
		if (!entry)
			return;

		auto& to_slot = m_slots[to_node_uid];
		const auto& from_slot = m_slots[from_node_uid];

		// Get output type from the from node
		const auto& from_overload = from_slot.m_overload;
		const auto from_type = from_overload.return_type();

		// Set the input type of the to node overload
		auto current_to_overload = to_slot.m_overload;
		current_to_overload.set_type(to_input, from_type);

		// Check if the overload is valid
		const auto& all_overloads = entry->m_ctor->overloads();
		if (std::find(all_overloads.begin(), all_overloads.end(), current_to_overload) == all_overloads.end())
			return;

		// Update the overload
		to_slot.m_overload = current_to_overload;

		// Add the connection
		m_connections.emplace_back(Connection{ from_node_uid, from_output, to_node_uid, to_input });
	}
}   // namespace shadergraph
