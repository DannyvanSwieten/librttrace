#pragma once

namespace shadergraph {
	class Connection
	{
	public:
		size_t from_node;
		size_t from_output;
		size_t to_node;
		size_t to_input;

		bool operator==(const Connection& other) const = default;
	};
}   // namespace shadergraph