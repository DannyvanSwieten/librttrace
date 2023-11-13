#pragma once
#include <vector>
#include "../bounding_box.hpp"
class VertexBuffer;
class IndexBuffer;
class CpuAccelerationStructure
{
public:
	CpuAccelerationStructure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer);
	void build(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer);

private:
	struct Node
	{
		BoundingBox m_bounding_box;
		uint32_t m_first_primitive;
		uint32_t m_primitive_count;
	};

	struct Split
	{
		uint32_t m_axis;
		float m_position;
		float m_cost;
	};

	void update_bounds(size_t node_index, const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer);
	void subdivide(size_t node_index, const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer);
	float evaluate_sah(
		const Node& node, const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer, uint32_t axis, float position);
	Split find_split(const Node& node, const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer);

	std::vector<uint32_t> m_triangle_indices;
	std::vector<Float3> m_triangle_centroids;
	std::vector<Node> m_nodes;
	uint32_t m_nodes_used = 1;
};