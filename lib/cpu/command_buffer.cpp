#include "command_buffer.hpp"
#include "bottom_level_acceleration_structure.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "../resource_ctx.hpp"
#include "top_level_acceleration_structure.hpp"
#include "frame_buffer.hpp"

Result<Void> CpuCommandBuffer::build_bottom_level_acceleration_structure(BottomLevelAccelerationStructure* const acceleration_structure,
                                                                         const VertexBuffer* const vertex_buffer,
                                                                         size_t vertex_stride,
                                                                         const IndexBuffer* const index_buffer)
{
	auto cpu_vertex_buffer = static_cast<const CpuVertexBuffer* const>(vertex_buffer);
	auto cpu_index_buffer = static_cast<const CpuIndexBuffer* const>(index_buffer);
	auto cpu_acceleration_structure = static_cast<CpuBottomLevelAccelerationStructure* const>(acceleration_structure);
	cpu_acceleration_structure->build(*cpu_vertex_buffer, vertex_stride, *cpu_index_buffer);
	return Result<Void>::from_value(Void());
}

Result<Void> CpuCommandBuffer::build_top_level_acceleration_structure(TopLevelAccelerationStructure* const acceleration_structure,
                                                                      const Instance* const instances,
                                                                      size_t count)
{
	return Result<Void>::from_value(Void());
}

Result<Void> CpuCommandBuffer::trace_rays(ResourceContext* resource_ctx, uint32_t x, uint32_t y)
{
	const auto pixel_size_x = 1.0f / x;
	const auto pixel_size_y = 1.0f / y;

	for (uint32_t j = 0; j < y; j++)
	{
		const auto fy = -1.0f + 2.0f * (static_cast<float>(j) + 0.5f) * pixel_size_y;
		for (uint32_t i = 0; i < x; i++)
		{
			const auto fx = -1.0f + 2.0f * (static_cast<float>(i) + 0.5f) * pixel_size_x;
			Float3 ray_origin = { 0.0f, 0.0f, -1.0f };
			Float3 pixel_location = { fx, fy, 1.0f };
			Float3 ray_direction = pixel_location - ray_origin;
			ray_direction = normalize(ray_direction);
			for (const auto& [key, acceleration_structure] : resource_ctx->acceleration_structures())
			{
				const auto cpu_top_level_acceleration_structure = static_cast<const CpuTopLevelAccelerationStructure* const>(acceleration_structure);
				const auto record = cpu_top_level_acceleration_structure->intersect(ray_origin, ray_direction);
				if (record.primitive_id != -1)
				{
					resource_ctx->get_frame_buffer(0)->set_pixel(i, j, 1.0, 1.0, 1.0, 1.0);
				}
			}
		}
	}
	return Result<Void>::from_value(Void());
}
