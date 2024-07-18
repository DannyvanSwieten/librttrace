#pragma once
#include <memory>
#include <map>
#include <string_view>
#include <vector>
#include "connection.hpp"
#include "overload.hpp"
#include "context.hpp"

namespace shadergraph {
	class Constructor;
	class ShaderGraph;
	class ConstructorCache;

	class Document
	{
	public:
		Document(ConstructorCache& registry);
		void add_node(size_t node_uid, const std::string& ctor_uid);
		void connect(size_t from_node_uid, size_t from_slot, size_t to_node_uid, size_t to_slot);
		void set_constant(size_t node_uid, size_t slot, const Value& value);

	private:
		struct Slot
		{
			std::string m_ctor_name;
			Overload m_overload;
			IOContext m_io_ctx;
		};

		ConstructorCache& m_registry;
		std::map<size_t, Slot> m_slots;
		std::vector<Connection> m_connections;
	};
}   // namespace shadergraph