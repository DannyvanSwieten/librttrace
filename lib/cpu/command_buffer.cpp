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
#include "vm.hpp"

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
	std::vector<std::function<void()>> jobs;
	const auto num_warps = y / std::thread::hardware_concurrency();
	const auto rows_per_warp = y / num_warps;

	std::atomic<uint32_t> counter = 0;

	instructions::ShaderProgram program;
	program.add_instruction(instructions::Div{ vm::globals::THREAD_ID, vm::globals::THREAD_COUNT, Register{ 0 } });
	program.add_instruction(instructions::Mul{ Register{ 0 }, 2.0, Register{ 0 } });
	program.add_instruction(instructions::Sub{ Register{ 0 }, Float3(1, 1, 0), Register{ 0 } });
	program.add_instruction(instructions::Load{ Register{ 0 }, Register{ 1 } });
	program.add_instruction(instructions::Sub{ Register{ 1 }, Float3(0, 0, -1), Register{ 1 } });
	program.add_instruction(instructions::Normalize{ Register{ 1 }, Register{ 1 } });
	program.add_instruction(instructions::Store{ Float3(0, 0, -1), vm::globals::RAY_ORIGIN });
	program.add_instruction(instructions::Store{ Register{ 1 }, vm::globals::RAY_DIRECTION });
	VirtualMachine vm;

	for (auto warp = 0; warp < num_warps; warp++)
	{
		const auto start_row = warp * rows_per_warp;
		const auto end_row = start_row + rows_per_warp;
		jobs.emplace_back([&, start_row, end_row] {
			for (uint32_t j = start_row; j < end_row; j++)
			{
				for (uint32_t i = 0; i < x; i++)
				{
					std::array<Value, 4> globals;
					vm.execute(program, globals, i, y - 1 - j, x, y);
					const auto ray_origin = get_value(globals[vm::globals::RAY_ORIGIN.name], {}, globals);
					const auto ray_direction = get_value(globals[vm::globals::RAY_DIRECTION.name], {}, globals);
					for (const auto& acceleration_structure : resource_ctx->acceleration_structures())
					{
						const auto cpu_top_level_acceleration_structure = static_cast<const CpuTopLevelAccelerationStructure* const>(acceleration_structure);
						const auto record = cpu_top_level_acceleration_structure->intersect(ray_origin, ray_direction);
						if (record.primitive_id != -1)
						{
							resource_ctx->get_frame_buffer(0)->set_pixel(i, j, record.u, record.v, 1.0 - record.u - record.v, 1.0);
						}
					}
				}
			}
		});
	}

	std::vector<std::thread> threads;
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	for (int t = 0; t < std::thread::hardware_concurrency(); t++)
	{
		threads.emplace_back(std::thread([&] {
			while (counter < jobs.size())
			{
				auto& job = jobs[counter++];
				job();
			}
		}));
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_time - start_time;
	printf("Elapsed time in ms: %f\n", elapsed_seconds.count() * 1000);

	return Result<Void>::from_value(Void());
}
