#pragma once
#include <map>
#include <memory>
#include <vector>
#include "instructions.hpp"
#include "context.hpp"
#include "overload.hpp"
#include "connection.hpp"

namespace shadergraph {

	class Node;
	class ShaderGraph
	{
	public:
		ShaderGraph() = default;
		ShaderGraph(ShaderGraph&& other);
		~ShaderGraph();

		size_t add_node(std::unique_ptr<Node> node);
		void connect(const Connection& connection);
		void disconnect(const Connection& connection);

		void set_default_value(size_t node_id, size_t output_id, const Operand& Operand);

		std::vector<Instruction> generate_ir();

		Operand get_output_Operand_for_connection(const Connection& connection) const
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
