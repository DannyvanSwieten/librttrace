#include "vm.hpp"
#include <array>
#include <iostream>
#include "../resource_ctx.hpp"
#include "top_level_acceleration_structure.hpp"
#include "ray_generation_program.hpp"
#include "closest_hit_program.hpp"
#include "frame_buffer.hpp"
#include "../pipeline.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "../instance.hpp"
template <class... Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void VirtualMachine::execute(
	const Pipeline* pipeline, ResourceContext* ctx, vm::globals::Globals& globals, int thread_id_x, int thread_id_y, int thread_count_x, int thread_count_y)
{
	const auto ray_gen = pipeline->ray_generation_program();
	const auto cpu_ray_gen = dynamic_cast<const CpuRayGenerationProgram*>(ray_gen);
	execute(cpu_ray_gen, pipeline, ctx, globals, thread_id_x, thread_id_y, thread_count_x, thread_count_y);
}

void VirtualMachine::execute(const instructions::ShaderProgram& program,
                             const Pipeline* pipeline,
                             ResourceContext* ctx,
                             vm::globals::Globals& globals,
                             int thread_id_x,
                             int thread_id_y,
                             int thread_count_x,
                             int thread_count_y)
{
	globals[vm::globals::THREAD_ID.name] = Float3{ static_cast<float>(thread_id_x), static_cast<float>(thread_id_y), 0.0f };
	globals[vm::globals::THREAD_COUNT.name] = Float3{ static_cast<float>(thread_count_x), static_cast<float>(thread_count_y), 1.0f };
	for (const auto& instruction : program.instructions)
	{
		std::visit(overloaded{ [&](const instructions::Add& add) {
								  const auto a = get_value(add.a, registers, globals);
								  const auto b = get_value(add.b, registers, globals);
								  const auto result = a + b;
								  // std::cout << "Op Add: r" << instruction.dst.index << " = " << a << " + " << b << std::endl;
								  registers[instruction.dst.index] = result;
								  // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							  },
		                       [&](const instructions::Sub& sub) {
								   const auto a = get_value(sub.a, registers, globals);
								   const auto b = get_value(sub.b, registers, globals);
								   const auto result = a - b;
								   // std::cout << "Op Sub: r" << instruction.dst.index << " = " << a << " - " << b << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const instructions::Mul& mul) {
								   const auto a = get_value(mul.a, registers, globals);
								   const auto b = get_value(mul.b, registers, globals);
								   const auto result = a * b;
								   // std::cout << "Op Mul: r" << instruction.dst.index << " = " << a << " * " << b << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const instructions::Div& div) {
								   const auto a = get_value(div.a, registers, globals);
								   const auto b = get_value(div.b, registers, globals);
								   const auto result = a / b;
								   // std::cout << "Op Div: r" << instruction.dst.index << " = " << a << " / " << b << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const instructions::Dot& dot) {
								   const auto a = get_value(dot.a, registers, globals);
								   const auto b = get_value(dot.b, registers, globals);
								   const auto result = glm::dot(a, b);
								   // std::cout << "Op Dot: r" << instruction.dst.index << " = " << a << " . " << b << std::endl;
								   registers[instruction.dst.index] = Float3(result);
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const instructions::Cross& cross) {
								   const auto a = get_value(cross.a, registers, globals);
								   const auto b = get_value(cross.b, registers, globals);
								   const auto result = glm::cross(a, b);
								   // std::cout << "Op Cross: r" << instruction.dst.index << " = " << a << " x " << b << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const instructions::Neg& neg) {
								   // std::cout << "Neg" << std::endl;
							   },
		                       [&](const instructions::Load& load) {
								   const auto src = get_value(load.src, registers, globals);
								   // std::cout << "Op Load: r" << instruction.dst.index << " with " << src << std::endl;
								   registers[instruction.dst.index] = src;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const instructions::LoadAttribute& load_att) {
								   const auto instance_id = static_cast<size_t>(get_value(globals[vm::globals::INSTANCE_ID.name], registers, globals).x);
								   const auto buffer_index = static_cast<size_t>(get_value(load_att.buffer_index, registers, globals).x);
								   const auto primitive_index = static_cast<size_t>(get_value(globals[vm::globals::PRIMITIVE_ID.name], registers, globals).x);
								   const auto vertex_index = static_cast<size_t>(get_value(load_att.vertex_index, registers, globals).x);
								   const auto acceleration_structure = ctx->get_acceleration_structure(0);
								   const auto cpu_acceleration_structure = dynamic_cast<const CpuTopLevelAccelerationStructure*>(acceleration_structure);
								   const auto& instance = cpu_acceleration_structure->instance(instance_id);

								   const auto cpu_vertex_buffer = dynamic_cast<const CpuVertexBuffer*>(instance.m_attribute_buffers[buffer_index]);
								   const auto cpu_index_buffer = dynamic_cast<const CpuIndexBuffer*>(instance.m_index_buffer);
								   const auto index = cpu_index_buffer->operator[](primitive_index + vertex_index);
								   const auto& vertex = cpu_vertex_buffer->float3(index * 3);
								   registers[instruction.dst.index] = vertex;
							   },
		                       [&](const instructions::Store& store) {
								   const auto src = get_value(store.src, registers, globals);
								   // std::cout << "Op Store: " << get_value(store.src, registers, globals) << " -> " << store.name.index << std::endl;
								   registers[store.name.index] = src;
								   // std::cout << "r " << store.name.index << " == " << get_value(registers[store.name.index], registers, globals) <<
			                       // std::endl;
							   },
		                       [&](const instructions::StorePixel& store_pixel) {
								   const auto image_index = static_cast<uint32_t>(get_value(store_pixel.image_index, registers, globals)[0]);
								   const auto location = get_value(store_pixel.location, registers, globals);
								   const auto color = get_value(store_pixel.color, registers, globals);
								   ctx->get_frame_buffer(image_index)
									   ->set_pixel(static_cast<uint32_t>(location[0]), static_cast<uint32_t>(location[1]), color[0], color[1], color[2], 1.0f);
							   },
		                       [&](const instructions::Normalize& norm) {
								   const auto src = get_value(norm.src, registers, globals);
								   const auto result = normalize(src);
								   // std::cout << "Op Normalize: r" << instruction.dst.index << " = "
			                       //    << "normalize( " << src << " )" << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const instructions::Intersect& intersect) {
								   const auto& ray_origin = get_value(intersect.origin, registers, globals);
								   const auto& ray_direction = get_value(intersect.direction, registers, globals);
								   const auto accel_index = static_cast<uint32_t>(get_value(intersect.acc_index, registers, globals)[0]);

								   const auto tlas = ctx->get_acceleration_structure(accel_index);
								   auto cpu_tlas = dynamic_cast<const CpuTopLevelAccelerationStructure*>(tlas);
								   const auto intersection_result = cpu_tlas->intersect(ray_origin, ray_direction);

								   globals[vm::globals::RAY_ORIGIN.name] = ray_origin;
								   globals[vm::globals::RAY_DIRECTION.name] = ray_direction;

								   if (intersection_result.is_hit())
								   {
									   const auto& [t, u, v, instance_id, primitive_id] = intersection_result;
									   globals[vm::globals::HIT_DISTANCE.name] = Float3{ t, t, t };
									   globals[vm::globals::BARYCENTRIC_COORDINATE.name] = Float3{ u, v, 1.0f - u - v };
									   globals[vm::globals::INSTANCE_ID.name] =
										   Float3{ static_cast<float>(instance_id), static_cast<float>(instance_id), static_cast<float>(instance_id) };
									   globals[vm::globals::PRIMITIVE_ID.name] =
										   Float3{ static_cast<float>(primitive_id), static_cast<float>(primitive_id), static_cast<float>(primitive_id) };

									   const auto cpu_closest_hit_program = static_cast<const CpuClosestHitProgram*>(pipeline->closest_hit_program());
									   execute(cpu_closest_hit_program, pipeline, ctx, globals, thread_id_x, thread_id_y, thread_count_x, thread_count_y);
								   }
								   else
								   {
									   ctx->get_frame_buffer(0)->set_pixel(thread_id_x, thread_id_y, 0.0, 0.0, 0.0f, 1.0f);
								   }
							   } },
		           instruction.op_code);
	}
}