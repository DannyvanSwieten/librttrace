#include <array>
#include <algorithm>
#include <cassert>

#include "../float3.hpp"

#include "bottom_level_acceleration_structure.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "intersect.hpp"

CpuBottomLevelAccelerationStructure::CpuBottomLevelAccelerationStructure(const CpuVertexBuffer* const vertex_buffer,
                                                                         size_t vertex_stride,
                                                                         const CpuIndexBuffer* const index_buffer)
	: m_vertex_buffer(vertex_buffer)
	, m_index_buffer(index_buffer)
	, m_vertex_stride(static_cast<uint32_t>(vertex_stride))
{
	m_triangle_indices.reserve(index_buffer->size() / 3);
	m_triangle_centroids.reserve(index_buffer->size() / 3);
	m_nodes.reserve(index_buffer->size() / 3 * 2);
}

void CpuBottomLevelAccelerationStructure::build(const CpuVertexBuffer& vertex_buffer, size_t vertex_stride, const CpuIndexBuffer& index_buffer)
{
	for (size_t i = 0; i < index_buffer.size(); i += 3)
	{
		m_triangle_indices.emplace_back(static_cast<uint32_t>(i));

		// Add Two nodes
		m_nodes.emplace_back();
		m_nodes.emplace_back();

		auto i0 = index_buffer[i];
		auto i1 = index_buffer[i + 1];
		auto i2 = index_buffer[i + 2];

		auto index = i0 * vertex_stride;
		auto v0 = vertex_buffer.float3(index);
		index = i1 * vertex_stride;
		auto v1 = vertex_buffer.float3(index);
		index = i2 * vertex_stride;
		auto v2 = vertex_buffer.float3(index);

		m_triangle_centroids.emplace_back((v0 + v1 + v2) / 3.0f);
		m_bounding_box.grow(v0);
		m_bounding_box.grow(v1);
		m_bounding_box.grow(v2);
	}

	m_nodes[0].m_primitive_count = static_cast<uint32_t>(m_triangle_indices.size());
	update_bounds(0, vertex_buffer, vertex_stride, index_buffer);
	subdivide(0, vertex_buffer, vertex_stride, index_buffer);
	m_nodes.resize(m_nodes_used);
}

void CpuBottomLevelAccelerationStructure::update_bounds(size_t node_index,
                                                        const CpuVertexBuffer& vertex_buffer,
                                                        size_t vertex_stride,
                                                        const CpuIndexBuffer& index_buffer)
{
	auto& node = m_nodes[node_index];
	const auto first_primitive = node.m_first_primitive;
	const auto last_primitive = node.m_first_primitive + node.m_primitive_count;

	for (size_t i = first_primitive; i < last_primitive; ++i)
	{
		const auto triangle_index = m_triangle_indices[i];
		const auto i0 = index_buffer[triangle_index];
		const auto i1 = index_buffer[triangle_index + 1];
		const auto i2 = index_buffer[triangle_index + 2];

		auto index = i0 * vertex_stride;
		const auto v0 = vertex_buffer.float3(index);
		index = i1 * vertex_stride;
		const auto v1 = vertex_buffer.float3(index);
		index = i2 * vertex_stride;
		const auto v2 = vertex_buffer.float3(index);

		const auto minimum = min(min(v0, v1), v2);
		const auto maximum = max(max(v0, v1), v2);
		const auto local_bbox = BoundingBox(minimum, maximum);
		node.m_bounding_box.grow(local_bbox);
	}
}

void CpuBottomLevelAccelerationStructure::subdivide(size_t node_index,
                                                    const CpuVertexBuffer& vertex_buffer,
                                                    size_t vertex_stride,
                                                    const CpuIndexBuffer& index_buffer)
{
	auto& node = m_nodes[node_index];
	const auto split = find_split(node, vertex_buffer, vertex_stride, index_buffer);
	const auto parent_area = node.m_bounding_box.area();
	const auto parent_cost = parent_area * node.m_primitive_count;
	if (split.m_cost >= parent_cost)
	{
		return;
	}

	auto i = node.m_first_primitive;
	auto j = i + node.m_primitive_count - 1;
	while (i <= j)
	{
		const auto triangle_index = m_triangle_indices[i];
		const auto& triangle_centroid = m_triangle_centroids[triangle_index / 3];
		if (triangle_centroid[split.m_axis] < split.m_position)
		{
			++i;
		}
		else
		{
			std::iter_swap(m_triangle_indices.begin() + i, m_triangle_indices.begin() + j);
			--j;
		}
	}

	const auto left_count = i - node.m_first_primitive;
	if (left_count == 0 || left_count == node.m_primitive_count)
	{
		return;
	}

	const auto left_node_index = m_nodes_used++;
	const auto right_node_index = m_nodes_used++;

	auto& left_node = m_nodes[left_node_index];
	left_node.m_first_primitive = node.m_first_primitive;
	left_node.m_primitive_count = left_count;

	auto& right_node = m_nodes[right_node_index];
	right_node.m_first_primitive = i;
	right_node.m_primitive_count = node.m_primitive_count - left_count;

	node.m_first_primitive = left_node_index;
	node.m_primitive_count = 0;

	update_bounds(left_node_index, vertex_buffer, vertex_stride, index_buffer);
	update_bounds(right_node_index, vertex_buffer, vertex_stride, index_buffer);

	subdivide(left_node_index, vertex_buffer, vertex_stride, index_buffer);
	subdivide(right_node_index, vertex_buffer, vertex_stride, index_buffer);
}

float CpuBottomLevelAccelerationStructure::evaluate_sah(
	const Node& node, const CpuVertexBuffer& vertex_buffer, size_t vertex_stride, const CpuIndexBuffer& index_buffer, uint32_t axis, float position)
{
	BoundingBox left_bb;
	BoundingBox right_bb;

	auto left_count = 0;
	auto right_count = 0;

	const auto first = node.m_first_primitive;
	const auto last = node.m_first_primitive + node.m_primitive_count;

	for (uint32_t i = first; i < last; ++i)
	{
		const auto triangle_index = m_triangle_indices[i];
		const auto& triangle_centroid = m_triangle_centroids[triangle_index / 3];
		auto i0 = index_buffer[triangle_index];
		auto i1 = index_buffer[triangle_index + 1];
		auto i2 = index_buffer[triangle_index + 2];

		auto index = i0 * vertex_stride;
		auto v0 = vertex_buffer.float3(index);
		index = i1 * vertex_stride;
		auto v1 = vertex_buffer.float3(index);
		index = i2 * vertex_stride;
		auto v2 = vertex_buffer.float3(index);

		if (triangle_centroid[static_cast<size_t>(axis)] < position)
		{
			++left_count;
			left_bb.grow(v0);
			left_bb.grow(v1);
			left_bb.grow(v2);
		}
		else
		{
			++right_count;
			right_bb.grow(v0);
			right_bb.grow(v1);
			right_bb.grow(v2);
		}
	}

	const auto cost = left_count * left_bb.area() + right_count * right_bb.area();
	if (cost > 0)
	{
		return cost;
	}
	else
	{
		return std::numeric_limits<float>::max();
	}
}

CpuBottomLevelAccelerationStructure::Split CpuBottomLevelAccelerationStructure::find_split(const Node& node,
                                                                                           const CpuVertexBuffer& vertex_buffer,
                                                                                           size_t vertex_stride,
                                                                                           const CpuIndexBuffer& index_buffer)
{
	uint32_t best_axis = 0;
	auto best_position = 0.0f;
	auto best_cost = std::numeric_limits<float>::max();

	for (auto axis = 0; axis < 3; ++axis)
	{
		const auto minimum = node.m_bounding_box.m_min[axis];
		const auto maximum = node.m_bounding_box.m_max[axis];
		if (minimum == maximum)
		{
			continue;
		}

		const auto bin_count = 10;
		const auto scale = node.m_bounding_box.size()[axis] / bin_count;
		for (auto i = 0; i < bin_count; ++i)
		{
			const auto candidate = minimum + scale * i;
			const auto cost = evaluate_sah(node, vertex_buffer, vertex_stride, index_buffer, axis, candidate);
			if (cost < best_cost)
			{
				best_axis = axis;
				best_position = candidate;
				best_cost = cost;
			}
		}
	}

	return Split{ best_axis, best_position, best_cost };
}

void CpuBottomLevelAccelerationStructure::intersect(const Mat4x4& transform, const Float3& origin, const Float3& dir, float t_min, HitRecord& record) const
{
	const auto t_max = std::numeric_limits<float>::max();
	const auto inv_transform = glm::inverse(transform);
	const auto inv_origin = inv_transform * Float4(origin, 1.0);
	const auto inv_dir = inv_transform * Float4(dir, 0.0);

	if (!intersect_bounding_box(inv_origin, inv_dir, m_bounding_box, t_min, t_max))
	{
		return;
	}

	// Intersect bvh
	std::array<uint32_t, 32> stack;
	uint32_t node_index = 0;
	uint32_t stack_ptr = 0;

	while (true)
	{
		const auto& node = m_nodes[node_index];
		if (node.m_primitive_count > 0)
		{
			for (uint32_t i = 0; i < node.m_primitive_count; ++i)
			{
				const auto triangle_index = m_triangle_indices[node.m_first_primitive + i];
				const auto i0 = m_index_buffer->operator[](triangle_index) * m_vertex_stride;
				const auto i1 = m_index_buffer->operator[](triangle_index + 1) * m_vertex_stride;
				const auto i2 = m_index_buffer->operator[](triangle_index + 2) * m_vertex_stride;
				const auto v0 = Float3(m_vertex_buffer->operator[](i0), m_vertex_buffer->operator[](i0 + 1), m_vertex_buffer->operator[](i0 + 2));
				const auto v1 = Float3(m_vertex_buffer->operator[](i1), m_vertex_buffer->operator[](i1 + 1), m_vertex_buffer->operator[](i1 + 2));
				const auto v2 = Float3(m_vertex_buffer->operator[](i2), m_vertex_buffer->operator[](i2 + 1), m_vertex_buffer->operator[](i2 + 2));
				float t, u, v;
				// Mat3x4 tr = transform;
				// const auto vt0 = transform * Float4(v0, 1.0);
				// const auto vt1 = transform * Float4(v1, 1.0);
				// const auto vt2 = transform * Float4(v2, 1.0);
				if (intersect_muller_trumbore(inv_origin, inv_dir, v0, v1, v2, t, u, v))
				{
					if (t > 0.01 && t < record.t)
					{
						record.t = t;
						record.u = u;
						record.v = v;
						record.primitive_id = triangle_index;
					}
				}
			}

			if (stack_ptr == 0)
			{
				break;
			}
			else
			{
				node_index = stack[--stack_ptr];
				continue;
			}
		}
		else
		{
			const auto left = node.m_first_primitive;
			const auto right = left + 1;
			const auto& left_bb = m_nodes[left].m_bounding_box;
			const auto& right_bb = m_nodes[right].m_bounding_box;
			float t_left, t_right;

			const auto hit_left = intersect_bounding_box(inv_origin, inv_dir, left_bb, t_left, t_max);
			const auto hit_right = intersect_bounding_box(inv_origin, inv_dir, right_bb, t_right, t_max);
			if (hit_left && hit_right)
			{
				if (t_left < t_right)
				{
					stack[stack_ptr++] = right;
					node_index = left;
				}
				else
				{
					stack[stack_ptr++] = left;
					node_index = right;
				}
			}
			else if (hit_left)
			{
				node_index = left;
			}
			else if (hit_right)
			{
				node_index = right;
			}
			else
			{
				if (stack_ptr == 0)
				{
					break;
				}
				node_index = stack[--stack_ptr];
			}
		}
	}
}

Instance CpuBottomLevelAccelerationStructure::create_instance(uint32_t instance_id, InstanceMask mask, const Mat4x4& transform)
{
	return { m_index_buffer, { m_vertex_buffer, nullptr, nullptr, nullptr }, this, instance_id, mask, transform };
}

const BoundingBox& CpuBottomLevelAccelerationStructure::bounding_box() const
{
	return m_bounding_box;
}
