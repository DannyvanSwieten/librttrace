#pragma once
#include <map>
#include <memory>
#include <vector>
#include "../compiler/instructions.hpp"
#include "context.hpp"
namespace shadergraph {
	class Node;
	class ShaderGraph
	{
	public:
		~ShaderGraph();
		struct Connection
		{
			size_t from_node;
			size_t from_output;
			size_t to_node;
			size_t to_input;

			bool operator==(const Connection& other) const = default;
		};

		size_t add_node(std::unique_ptr<Node> node);
		void connect(const Connection& connection);
		void disconnect(const Connection& connection);

		void set_default_value(size_t node_id, size_t output_id, const Value& value);

		instructions::ShaderProgram generate_ir();

	private:
		void topological_sort();

	private:
		CompilerContext m_compiler_ctx;

		std::map<size_t, std::unique_ptr<Node>> m_nodes;
		std::map<size_t, IOContext> m_io_contexts;
		std::vector<Connection> m_connections;
		std::vector<size_t> m_sorted_nodes;
		size_t m_next_free_index = 0;
	};
}   // namespace shadergraph
