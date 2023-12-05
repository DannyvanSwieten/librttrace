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
		ShaderGraph() = default;
		ShaderGraph(size_t next_free_register);
		ShaderGraph(ShaderGraph&& other);
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

		size_t next_free_register() const
		{
			return m_compiler_ctx.m_next_free_register;
		}

		Value get_output_value_for_connection(const Connection& connection) const
		{
			const auto& io_ctx = m_io_contexts.at(connection.from_node);
			return io_ctx.output(connection.from_output).dst;
		}

		std::vector<Connection> get_every_node_connected_from(size_t output_node_id) const
		{
			std::vector<Connection> connections;
			for (const auto& connection : m_connections)
			{
				if (connection.from_node == output_node_id)
				{
					connections.push_back(connection);
				}
			}

			return connections;
		}

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
