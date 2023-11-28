#include <device_factory.hpp>
#include <device.hpp>
#include <array>

#include "bottom_level_acceleration_structure.hpp"
#include "resource_ctx.hpp"
#include "image.hpp"
#include "compiler/instructions.hpp"
#include "cpu/vm.hpp"

constexpr uint32_t width = 512;
constexpr uint32_t height = 512;

int main()
{
	DeviceFactory factory;
	auto device = factory.create_device(Api::CPU).expect("Failed to create device");
	printf("Device created: %s\n", device->vendor_id().value());

	auto frame_buffer = device->alloc_frame_buffer(PixelFormat::RGBA32SFLOAT, width, height).expect("Failed to create frame buffer");

	const auto triangle_vertices = std::array<float, 9>{ -0.5f, -0.5f, 1.0f, 0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 1.0f };
	const auto vertex_buffer = device->alloc_vertex_buffer(triangle_vertices.data(), triangle_vertices.size()).expect("Failed to create vertex buffer");

	const auto triangle_indices = std::array<uint32_t, 3>{ 0, 1, 2 };
	const auto index_buffer = device->alloc_index_buffer(triangle_indices.data(), triangle_indices.size()).expect("Failed to create index buffer");

	const auto acceleration_structure =
		device->alloc_bottom_level_acceleration_structure(vertex_buffer, 3, index_buffer).expect("Failed to create acceleration structure");

	auto command_buffer = device->alloc_command_buffer().expect("Failed to create command buffer");
	command_buffer->build_bottom_level_acceleration_structure(acceleration_structure, vertex_buffer, 3, index_buffer);

	const auto instance = acceleration_structure->create_instance(0, InstanceMask::Opaque, nullptr);

	auto tlas = device->alloc_top_level_acceleration_structure(&instance, 1).expect("Failed to create top level acceleration structure");

	command_buffer->build_top_level_acceleration_structure(tlas, &instance, 1);

	ResourceContext resource_ctx;
	resource_ctx.add_frame_buffer(0, frame_buffer);
	resource_ctx.add_acceleration_structure(0, tlas);

	command_buffer->trace_rays(&resource_ctx, width, height);

	const auto frame_buffer_data = frame_buffer->data().expect("Failed to get frame buffer data");

	Image image(static_cast<const float*>(frame_buffer_data), width, height, 4);
	image.write("output.hdr");

	return 0;
}