#include "../float3.hpp"

#include "bottom_level_acceleration_structure.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

CpuBottomLevelAccelerationStructure::CpuBottomLevelAccelerationStructure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer)
{
	const auto cpu_vertex_buffer = dynamic_cast<const CpuVertexBuffer&>(*vertex_buffer);
	const auto cpu_index_buffer = dynamic_cast<const CpuIndexBuffer&>(*index_buffer);

	m_triangle_indices.reserve(cpu_index_buffer.size() / 3);
	m_triangle_centroids.reserve(cpu_index_buffer.size() / 3);
    m_nodes.reserve(cpu_index_buffer.size() / 3 * 2);
}

void CpuBottomLevelAccelerationStructure::build(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer)
{
    const auto cpu_vertex_buffer = dynamic_cast<const CpuVertexBuffer&>(*vertex_buffer);
	const auto cpu_index_buffer = dynamic_cast<const CpuIndexBuffer&>(*index_buffer);

    for (size_t i = 0; i < cpu_index_buffer.size(); i += 3)
	{
		m_triangle_indices.emplace_back(static_cast<uint32_t>(i));

		// Add Two nodes
		m_nodes.emplace_back();
		m_nodes.back().m_bounding_box = BoundingBox::infinite();
		m_nodes.emplace_back();
		m_nodes.back().m_bounding_box = BoundingBox::infinite();

		auto i0 = cpu_index_buffer[i];
		auto i1 = cpu_index_buffer[i + 1];
		auto i2 = cpu_index_buffer[i + 2];

		auto index = i0 * vertex_stride;
		auto v0 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);
		index = i1 * vertex_stride;
		auto v1 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);
		index = i2 * vertex_stride;
		auto v2 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);

		m_triangle_centroids.emplace_back((v0 + v1 + v2) / 3.0f);
	}

	m_nodes[0].m_primitive_count = static_cast<uint32_t>(m_triangle_indices.size());
    update_bounds(0, vertex_buffer, vertex_stride, index_buffer);
    subdivide(0, vertex_buffer, vertex_stride, index_buffer);
}

void CpuBottomLevelAccelerationStructure::update_bounds(size_t node_index,
                                             const VertexBuffer* const vertex_buffer,
                                             size_t vertex_stride,
                                             const IndexBuffer* const index_buffer)
{
	const auto cpu_vertex_buffer = dynamic_cast<const CpuVertexBuffer&>(*vertex_buffer);
	const auto cpu_index_buffer = dynamic_cast<const CpuIndexBuffer&>(*index_buffer);

	auto& node = m_nodes[node_index];
	const auto first_primitive = node.m_first_primitive;
	const auto last_primitive = node.m_first_primitive + node.m_primitive_count;

	for (size_t i = first_primitive; i < last_primitive; ++i)
	{
		const auto triangle_index = m_triangle_indices[i];
		const auto triangle_centroid = m_triangle_centroids[triangle_index];
		auto i0 = cpu_index_buffer[triangle_index];
		auto i1 = cpu_index_buffer[triangle_index + 1];
		auto i2 = cpu_index_buffer[triangle_index + 2];

		auto index = i0 * vertex_stride;
		auto v0 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);
		index = i1 * vertex_stride;
		auto v1 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);
		index = i2 * vertex_stride;
		auto v2 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);

		const auto minimum = min(min(v0, v1), v2);
		const auto maximum = max(max(v0, v1), v2);
		const auto local_bbox = BoundingBox(minimum, maximum);
		node.m_bounding_box.grow(local_bbox);
	}
}

void CpuBottomLevelAccelerationStructure::subdivide(size_t node_index,
                                         const VertexBuffer* const vertex_buffer,
                                         size_t vertex_stride,
                                         const IndexBuffer* const index_buffer)
{
	auto& node = m_nodes[node_index];
    const auto split = find_split(node, vertex_buffer, vertex_stride, index_buffer);
    const auto parent_area = node.m_bounding_box.area();
    const auto parent_cost = parent_area * node.m_primitive_count;
    if(split.m_cost >= parent_cost)
    {
        return;
    }

    auto i = node. m_first_primitive;
    auto j = node.m_first_primitive + node.m_primitive_count - 1;
    while(i <= j)
    {
        const auto triangle_index = m_triangle_indices[i];
        const auto triangle_centroid = m_triangle_centroids[triangle_index];
        if(triangle_centroid[split.m_axis] < split.m_position)
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
    if(left_count == 0 || left_count == node.m_primitive_count)
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
	const Node& node, const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer, uint32_t axis, float position)
{
    const auto cpu_vertex_buffer = dynamic_cast<const CpuVertexBuffer&>(*vertex_buffer);
	const auto cpu_index_buffer = dynamic_cast<const CpuIndexBuffer&>(*index_buffer);

	auto left_bb = BoundingBox::infinite();
    auto right_bb = BoundingBox::infinite();

    auto left_count = 0;
    auto right_count = 0;

    const auto first = node.m_first_primitive;
    const auto last = node.m_first_primitive + node.m_primitive_count;

    for(uint32_t i = first; i < last; ++i)
    {
        const auto triangle_index = m_triangle_indices[i];
        const auto& triangle_centroid = m_triangle_centroids[triangle_index];
        auto i0 = cpu_index_buffer[triangle_index];
        auto i1 = cpu_index_buffer[triangle_index + 1];
        auto i2 = cpu_index_buffer[triangle_index + 2];

        auto index = i0 * vertex_stride;
        auto v0 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);
        index = i1 * vertex_stride;
        auto v1 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);
        index = i2 * vertex_stride;
        auto v2 = Float3(cpu_vertex_buffer[index], cpu_vertex_buffer[index + 1], cpu_vertex_buffer[index + 2]);

        if(triangle_centroid[static_cast<size_t>(axis)])
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
    if(cost > 0)
    {
        return cost;
    }
    else
    {
        return std::numeric_limits<float>::max();
    }
}

CpuBottomLevelAccelerationStructure::Split
CpuBottomLevelAccelerationStructure::find_split(const Node& node, const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer)
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

    return Split{best_axis, best_position, best_cost};
}
