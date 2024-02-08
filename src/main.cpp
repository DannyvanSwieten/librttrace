#include <device_factory.hpp>
#include <device.hpp>

#include "bottom_level_acceleration_structure.hpp"
#include "resource_ctx.hpp"
#include "image.hpp"
#include <cpu/virtual_machine/vm.hpp>

#include "shader_compiler.hpp"

#include "shaders.hpp"

#include "pipeline.hpp"

#include "cube.hpp"

constexpr uint32_t width = 512;
constexpr uint32_t height = 512;

int main()
{
	Api api = Api::CPU;
	DeviceFactory factory;
	auto device = factory.create_device(api).expect("Failed to create device");
	printf("Device created: %s\n", device->vendor_id().value());

	std::vector<float> vertices = create_cube_vertices();
	std::vector<uint32_t> indices = create_cube_indices();

	auto frame_buffer = device->alloc_frame_buffer(PixelFormat::RGBA32SFLOAT, width, height).expect("Failed to create frame buffer");

	const auto vertex_buffer =
		device->alloc_vertex_buffer(reinterpret_cast<const float*>(vertices.data()), vertices.size()).expect("Failed to create vertex buffer");

	const auto index_buffer = device->alloc_index_buffer(indices.data(), indices.size()).expect("Failed to create index buffer");

	const auto acceleration_structure =
		device->alloc_bottom_level_acceleration_structure(vertex_buffer, 3, index_buffer).expect("Failed to create acceleration structure");

	auto command_buffer = device->alloc_command_buffer().expect("Failed to create command buffer");
	command_buffer->build_bottom_level_acceleration_structure(acceleration_structure, vertex_buffer, 3, index_buffer);

	const auto instance = acceleration_structure->create_instance(0, InstanceMask::Opaque, Mat4x4(1.0));

	auto tlas = device->alloc_top_level_acceleration_structure(&instance, 1).expect("Failed to create top level acceleration structure");
	command_buffer->build_top_level_acceleration_structure(tlas, &instance, 1);

	auto ray_gen_graph = create_ray_gen_program();
	const auto ir = ray_gen_graph.generate_ir();
	auto closest_hit_graph = create_closest_hit_program();
	const auto ir2 = closest_hit_graph.generate_ir();
	ShaderCompiler compiler(api);
	auto ray_gen = compiler.compile_ray_generation_program(ir).expect("Failed to compile ray generation program");
	auto closest_hit = compiler.compile_closest_hit_program(ir2).expect("Failed to compile closest hit program");

	auto miss_graph = create_miss_program();
	const auto ir3 = miss_graph.generate_ir();
	auto miss = compiler.compile_miss_program(ir3).expect("Failed to compile miss program");

	Pipeline pipeline(ray_gen, closest_hit, miss, nullptr, 0);

	ResourceContext resource_ctx;
	resource_ctx.add_frame_buffer(0, frame_buffer);
	resource_ctx.add_acceleration_structure(0, tlas);

	command_buffer->trace_rays(&pipeline, &resource_ctx, width, height);

	const auto frame_buffer_data = frame_buffer->data().expect("Failed to get frame buffer data");

	Image image(static_cast<const float*>(frame_buffer_data), width, height, 4);
	image.write("output.hdr");

	return 0;
}