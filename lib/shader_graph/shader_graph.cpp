#include "shader_graph.hpp"
#include "context.hpp"
#include "node/node.hpp"
#include <queue>
namespace shadergraph {
	ShaderGraph::~ShaderGraph()
	{}

	size_t ShaderGraph::add_node(std::unique_ptr<Node> node)
	{
		size_t index = m_next_free_index++;
		m_nodes.emplace(index, std::move(node));
		m_io_contexts.emplace(index, IOContext{ m_compiler_ctx });
		m_nodes.at(index)->add_io(m_io_contexts.at(index));
		return index;
	}

	void ShaderGraph::connect(const Connection& connection)
	{
		m_connections.push_back(connection);
	}

	void ShaderGraph::disconnect(const Connection& connection)
	{
		auto it = std::find(m_connections.begin(), m_connections.end(), connection);
		if (it != m_connections.end())
		{
			m_connections.erase(it);
		}
	}

	void ShaderGraph::set_default_value(size_t node_id, size_t output_id, const Value& value)
	{
		auto& io_ctx = m_io_contexts.at(node_id);
		io_ctx.set_input_value(output_id, value);
	}

	instructions::ShaderProgram ShaderGraph::generate_ir()
	{
		topological_sort();
		instructions::ShaderProgram program;
		for (const auto& node_id : m_sorted_nodes)
		{
			auto& node = m_nodes.at(node_id);
			auto& io_ctx = m_io_contexts.at(node_id);
			node->output_instruction(m_compiler_ctx, io_ctx);
			for (const auto& [output_id, output] : io_ctx.output_values())
			{
				for (const auto& connection : m_connections)
				{
					if (connection.from_node == node_id)
					{
						auto& connected_node = m_nodes.at(connection.to_node);
						auto& connected_io_ctx = m_io_contexts.at(connection.to_node);
						connected_io_ctx.set_input_value(connection.to_input, output.instruction.dst);
					}
				}
				program.add_instruction(output.instruction);
			}
		}

		return program;
	}

	void ShaderGraph::topological_sort()
	{
		std::vector<std::vector<size_t>> adjacency_list(m_nodes.size());
		std::vector<size_t> indegree(m_nodes.size(), 0);
		for (const auto& connection : m_connections)
		{
			adjacency_list[connection.from_node].push_back(connection.to_node);
			indegree[connection.to_node]++;
		}

		std::queue<size_t> q;
		for (int i = 0; i < m_nodes.size(); i++)
		{
			if (indegree[i] == 0)
			{
				q.push(i);
			}
		}

		std::vector<size_t> top_order;
		while (!q.empty())
		{
			size_t node = q.front();
			q.pop();
			top_order.push_back(node);
			for (const auto& neighbour : adjacency_list[node])
			{
				indegree[neighbour]--;
				if (indegree[neighbour] == 0)
				{
					q.push(neighbour);
				}
			}
		}

		m_sorted_nodes = std::move(top_order);
	}
}   // namespace shadergraph