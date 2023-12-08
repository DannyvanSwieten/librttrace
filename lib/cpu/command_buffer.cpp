#include <thread>
#include <array>
#include <functional>
#include "command_buffer.hpp"
#include "bottom_level_acceleration_structure.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "../resource_ctx.hpp"
#include "top_level_acceleration_structure.hpp"
#include "frame_buffer.hpp"
#include "virtual_machine/vm.hpp"
#include "../pipeline.hpp"
#include "ray_generation_program.hpp"

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

Result<Void> CpuCommandBuffer::trace_rays(const Pipeline* pipeline, ResourceContext* resource_ctx, uint32_t x, uint32_t y)
{
	const auto ray_generation_program = pipeline->ray_generation_program();
	const auto cpu_ray_generation_program = static_cast<const CpuRayGenerationProgram* const>(ray_generation_program);

	std::vector<std::function<void()>> jobs;
	const auto num_warps = y;   // / std::thread::hardware_concurrency();
	const auto rows_per_warp = y / num_warps;

	std::atomic<uint32_t> counter = 0;
	auto fb = resource_ctx->get_frame_buffer(0);
	for (uint32_t warp = 0; warp < num_warps; warp++)
	{
		const auto start_row = warp * rows_per_warp;
		const auto end_row = start_row + rows_per_warp;
		jobs.emplace_back([&, start_row, end_row] {
			globals::Globals globals;
			VirtualMachine vm;
			for (uint32_t j = start_row; j < end_row; j++)
			{
				if (j > y)
					return;
				for (uint32_t i = 0; i < x; i++)
				{
					vm.execute(pipeline, resource_ctx, globals, i, y - 1 - j, x, y);
				}
			}
		});
	}

	std::vector<std::thread> threads;
	for (uint32_t t = 0; t < std::thread::hardware_concurrency(); t++)
	{
		threads.emplace_back(std::thread([&] {
			while (counter < jobs.size())
			{
				auto& job = jobs[counter++];
				job();
			}
		}));
	}

	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

	for (auto& thread : threads)
	{
		thread.join();
	}

	std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_time - start_time;
	printf("Elapsed time in ms: %f\n", elapsed_seconds.count() * 1000);

	return Result<Void>::from_value(Void());
}
