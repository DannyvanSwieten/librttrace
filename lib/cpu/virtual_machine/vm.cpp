#include "vm.hpp"
#include <array>
#include <iostream>
#include "../../resource_ctx.hpp"
#include "../top_level_acceleration_structure.hpp"
#include "../ray_generation_program.hpp"
#include "../closest_hit_program.hpp"
#include "../frame_buffer.hpp"
#include "../../pipeline.hpp"
#include "../vertex_buffer.hpp"
#include "../index_buffer.hpp"
#include "../../instance.hpp"
#include "../../shader_graph/value.hpp"
#include "../../shader_graph/instructions.hpp"
#include "../miss_program.hpp"

#include "../../overloaded.hpp"

void VirtualMachine::execute(
	const Pipeline* pipeline, ResourceContext* ctx, globals::Globals& globals, int thread_id_x, int thread_id_y, int thread_count_x, int thread_count_y)
{
	const auto ray_gen = pipeline->ray_generation_program();
	const auto cpu_ray_gen = dynamic_cast<const CpuRayGenerationProgram*>(ray_gen);
	execute(cpu_ray_gen, pipeline, ctx, globals, thread_id_x, thread_id_y, thread_count_x, thread_count_y);
}

void VirtualMachine::execute(const ShaderProgram& program,
                             const Pipeline* pipeline,
                             ResourceContext* ctx,
                             globals::Globals& globals,
                             int thread_id_x,
                             int thread_id_y,
                             int thread_count_x,
                             int thread_count_y)
{
	globals[globals::THREAD_ID.name] = Float3{ static_cast<float>(thread_id_x), static_cast<float>(thread_id_y), 0.0f };
	globals[globals::THREAD_COUNT.name] = Float3{ static_cast<float>(thread_count_x), static_cast<float>(thread_count_y), 1.0f };
	for (const auto& instruction : program.instructions)
	{
		std::visit(overloaded{ [&](const shadergraph::Add& add) {
								  const auto a = shadergraph::get_value(add.a, registers, globals);
								  const auto b = shadergraph::get_value(add.b, registers, globals);
								  const auto result = a + b;
								  // std::cout << "Op Add: r" << instruction.dst.index << " = " << a << " + " << b << std::endl;
								  registers[instruction.dst.index] = result;
								  // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							  },
		                       [&](const shadergraph::Sub& sub) {
								   const auto a = shadergraph::get_value(sub.a, registers, globals);
								   const auto b = shadergraph::get_value(sub.b, registers, globals);
								   const auto result = a - b;
								   // std::cout << "Op Sub: r" << instruction.dst.index << " = " << a << " - " << b << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const shadergraph::Mul& mul) {
								   const auto a = shadergraph::get_value(mul.a, registers, globals);
								   const auto b = shadergraph::get_value(mul.b, registers, globals);
								   const auto result = a * b;
								   // std::cout << "Op Mul: r" << instruction.dst.index << " = " << a << " * " << b << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const shadergraph::Div& div) {
								   const auto a = shadergraph::get_value(div.a, registers, globals);
								   const auto b = shadergraph::get_value(div.b, registers, globals);
								   const auto result = a / b;
								   // std::cout << "Op Div: r" << instruction.dst.index << " = " << a << " / " << b << std::endl;
								   registers[instruction.dst.index] = result;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const shadergraph::Dot& dot) {
								   const auto a = shadergraph::get_value(dot.a, registers, globals);
								   const auto b = shadergraph::get_value(dot.b, registers, globals);
								   const auto result = glm::dot(a, b);
								   // std::cout << "Op Dot: r" << instruction.dst.index << " = " << a << " . " << b << std::endl;
								   registers[instruction.dst.index] = Float4(result);
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const shadergraph::Cross& cross) {
								   const auto a = shadergraph::get_value(cross.a, registers, globals);
								   const auto b = shadergraph::get_value(cross.b, registers, globals);
								   const auto result = glm::cross(Float3(a), Float3(b));
								   // std::cout << "Op Cross: r" << instruction.dst.index << " = " << a << " x " << b << std::endl;
								   registers[instruction.dst.index] = Float4(result, 1.0);
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const shadergraph::Mix& mix) {
								   const auto a = shadergraph::get_value(mix.a, registers, globals);
								   const auto b = shadergraph::get_value(mix.b, registers, globals);
								   const auto c = shadergraph::get_value(mix.c, registers, globals);
								   const auto result = glm::mix(a, b, c.y);

								   // std::cout << "Op Mix: r" << instruction.dst.index << " = " << a << " mix " << b << " " << c << std::endl;
								   registers[instruction.dst.index] = result;
							   },
		                       [&](const shadergraph::SmoothStep& smooth_step) {
								   const auto a = shadergraph::get_value(smooth_step.a, registers, globals);
								   const auto b = shadergraph::get_value(smooth_step.b, registers, globals);
								   const auto c = shadergraph::get_value(smooth_step.c, registers, globals);
								   const auto result = glm::smoothstep(a.x, b.x, c.x);

								   // std::cout << "Op SmoothStep: r" << instruction.dst.index << " = " << a << " smoothstep " << b << " " << c << std::endl;
								   registers[instruction.dst.index] = Float4(result);
							   },
		                       [&](const shadergraph::Neg& neg) {
								   // std::cout << "Neg" << std::endl;
							   },
		                       [&](const shadergraph::Load& load) {
								   const auto src = shadergraph::get_value(load.src, registers, globals);
								   // std::cout << "Op Load: r" << instruction.dst.index << " with " << src << std::endl;
								   registers[instruction.dst.index] = src;
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const shadergraph::LoadAttribute& load_att) {
								   const auto instance_id =
									   static_cast<size_t>(shadergraph::get_value(globals[globals::INSTANCE_ID.name], registers, globals).x);
								   const auto buffer_index = static_cast<size_t>(shadergraph::get_value(load_att.buffer_index, registers, globals).x);
								   const auto primitive_index =
									   static_cast<size_t>(shadergraph::get_value(globals[globals::PRIMITIVE_ID.name], registers, globals).x);
								   const auto vertex_index = static_cast<size_t>(shadergraph::get_value(load_att.vertex_index, registers, globals).x);
								   const auto acceleration_structure = ctx->get_acceleration_structure(0);
								   const auto cpu_acceleration_structure = dynamic_cast<const CpuTopLevelAccelerationStructure*>(acceleration_structure);
								   const auto& instance = cpu_acceleration_structure->instance(instance_id);

								   const auto cpu_vertex_buffer = dynamic_cast<const CpuVertexBuffer*>(instance.m_attribute_buffers[buffer_index]);
								   const auto cpu_index_buffer = dynamic_cast<const CpuIndexBuffer*>(instance.m_index_buffer);
								   const auto index = cpu_index_buffer->operator[](primitive_index + vertex_index);
								   const auto& vertex = cpu_vertex_buffer->float3(index * 3);
								   registers[instruction.dst.index] = instance.m_transform * Float4(vertex, 1.0);
							   },
		                       [&](const shadergraph::Store& store) {
								   const auto src = shadergraph::get_value(store.src, registers, globals);
								   // std::cout << "Op Store: " << get_value(store.src, registers, globals) << " -> " << store.name.index << std::endl;
								   registers[store.name.index] = src;
								   // std::cout << "r " << store.name.index << " == " << get_value(registers[store.name.index], registers, globals) <<
			                       // std::endl;
							   },
		                       [&](const shadergraph::StorePixel& store_pixel) {
								   const auto image_index = static_cast<uint32_t>(shadergraph::get_value(store_pixel.image_index, registers, globals)[0]);
								   const auto location = shadergraph::get_value(store_pixel.location, registers, globals);
								   const auto color = shadergraph::get_value(store_pixel.color, registers, globals);
								   ctx->get_frame_buffer(image_index)
									   ->set_pixel(static_cast<uint32_t>(location[0]), static_cast<uint32_t>(location[1]), color[0], color[1], color[2], 1.0f);
							   },
		                       [&](const shadergraph::Normalize& norm) {
								   const auto src = shadergraph::get_value(norm.src, registers, globals);
								   const auto result = normalize(Float3(src));
								   // std::cout << "Op Normalize: r" << instruction.dst.index << " = "
			                       //    << "normalize( " << src << " )" << std::endl;
								   registers[instruction.dst.index] = Float4(result, 1.0);
								   // std::cout << "r" << instruction.dst.index << " == " << registers[instruction.dst.index] << std::endl;
							   },
		                       [&](const shadergraph::Length& length) {
								   const auto src = shadergraph::get_value(length.a, registers, globals);
								   const auto result = glm::length(Float3(src));
								   // std::cout << "Op Length: r" << instruction.dst.index << " = "
			                       //    << "length( " << src << " )" << std::endl;
								   registers[instruction.dst.index] = Float4(result);
							   },
		                       [&](const shadergraph::Abs& abs) {
								   const auto src = shadergraph::get_value(abs.a, registers, globals);
								   const auto result = glm::abs(Float3(src));
								   // std::cout << "Op Abs: r" << instruction.dst.index << " = "
			                       //    << "abs( " << src << " )" << std::endl;
								   registers[instruction.dst.index] = Float4(result, 1.0);
							   },
		                       [&](const shadergraph::Sin& sin) {
								   const auto src = shadergraph::get_value(sin.a, registers, globals);
								   const auto result = glm::sin(Float3(src));
								   // std::cout << "Op Sin: r" << instruction.dst.index << " = "
			                       //    << "sin( " << src << " )" << std::endl;
								   registers[instruction.dst.index] = Float4(result, 1.0);
							   },
		                       [&](const shadergraph::Cos& cos) {
								   const auto src = shadergraph::get_value(cos.a, registers, globals);
								   const auto result = glm::cos(Float3(src));
								   // std::cout << "Op Cos: r" << instruction.dst.index << " = "
			                       //    << "cos( " << src << " )" << std::endl;
								   registers[instruction.dst.index] = Float4(result, 1.0);
							   },
		                       [&](const shadergraph::Fract& fract) {
								   const auto src = shadergraph::get_value(fract.a, registers, globals);
								   const auto result = glm::fract(src);
								   // std::cout << "Op Fract: r" << instruction.dst.index << " = "
			                       //    << "fract( " << src << " )" << std::endl;
								   registers[instruction.dst.index] = result;
							   },
		                       [&](const shadergraph::Intersect& intersect) {
								   const auto& ray_origin = shadergraph::get_value(intersect.origin, registers, globals);
								   const auto& ray_direction = shadergraph::get_value(intersect.direction, registers, globals);
								   const auto accel_index = static_cast<uint32_t>(shadergraph::get_value(intersect.acc_index, registers, globals)[0]);

								   const auto tlas = ctx->get_acceleration_structure(accel_index);
								   auto cpu_tlas = dynamic_cast<const CpuTopLevelAccelerationStructure*>(tlas);
								   const auto intersection_result = cpu_tlas->intersect(ray_origin, ray_direction);

								   globals[globals::RAY_ORIGIN.name] = ray_origin;
								   globals[globals::RAY_DIRECTION.name] = ray_direction;

								   if (intersection_result.is_hit())
								   {
									   const auto& [t, u, v, instance_id, primitive_id] = intersection_result;
									   globals[globals::HIT_DISTANCE.name] = Float3{ t, t, t };
									   globals[globals::BARYCENTRIC_COORDINATE.name] = Float3{ u, v, 1.0f - u - v };
									   globals[globals::INSTANCE_ID.name] =
										   Float3{ static_cast<float>(instance_id), static_cast<float>(instance_id), static_cast<float>(instance_id) };
									   globals[globals::PRIMITIVE_ID.name] =
										   Float3{ static_cast<float>(primitive_id), static_cast<float>(primitive_id), static_cast<float>(primitive_id) };

									   const auto cpu_closest_hit_program = static_cast<const CpuClosestHitProgram*>(pipeline->closest_hit_program());
									   m_stack[m_stack_pointer++] = registers;
									   registers = m_stack[m_stack_pointer];
									   execute(cpu_closest_hit_program, pipeline, ctx, globals, thread_id_x, thread_id_y, thread_count_x, thread_count_y);
									   registers = m_stack[--m_stack_pointer];
								   }
								   else
								   {
									   const auto cpu_miss_program = static_cast<const CpuMissProgram*>(pipeline->miss_program());
									   if (cpu_miss_program)
									   {
										   m_stack[m_stack_pointer++] = registers;
										   registers = m_stack[m_stack_pointer];
										   execute(cpu_miss_program, pipeline, ctx, globals, thread_id_x, thread_id_y, thread_count_x, thread_count_y);
										   registers = m_stack[--m_stack_pointer];
									   }
								   }
							   } },
		           instruction.op_code);

		m_pc++;
	}
}