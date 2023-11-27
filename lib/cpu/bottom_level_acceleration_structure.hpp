#pragma once
#include <vector>
#include "../bounding_box.hpp"
#include "../bottom_level_acceleration_structure.hpp"
#include "intersect.hpp"
class CpuVertexBuffer;
class CpuIndexBuffer;

class CpuBottomLevelAccelerationStructure : public BottomLevelAccelerationStructure
{
public:
	CpuBottomLevelAccelerationStructure(const CpuVertexBuffer* const vertex_buffer, size_t vertex_stride, const CpuIndexBuffer* const index_buffer);
	void build(const CpuVertexBuffer& vertex_buffer, size_t vertex_stride, const CpuIndexBuffer& index_buffer);
	void intersect(const Float3& origin, const Float3& dir, float t_min, HitRecord& record) const;

	Instance create_instance(uint32_t instance_id, InstanceMask mask, const float* transform) override;

	const BoundingBox& bounding_box() const;

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

	void update_bounds(size_t node_index, const CpuVertexBuffer& vertex_buffer, size_t vertex_stride, const CpuIndexBuffer& index_buffer);
	void subdivide(size_t node_index, const CpuVertexBuffer& vertex_buffer, size_t vertex_stride, const CpuIndexBuffer& index_buffer);
	float evaluate_sah(
		const Node& node, const CpuVertexBuffer& vertex_buffer, size_t vertex_stride, const CpuIndexBuffer& index_buffer, uint32_t axis, float position);
	Split find_split(const Node& node, const CpuVertexBuffer& vertex_buffer, size_t vertex_stride, const CpuIndexBuffer& index_buffer);

	const CpuVertexBuffer* m_vertex_buffer;
	uint32_t m_vertex_stride;
	std::vector<uint32_t> m_triangle_indices;
	std::vector<Float3> m_triangle_centroids;
	std::vector<Node> m_nodes;
	uint32_t m_nodes_used = 1;

	BoundingBox m_bounding_box;
};