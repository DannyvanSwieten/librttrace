#include <device_factory.hpp>
#include <device.hpp>
#include <array>

int main()
{
	const auto triangle_vertices = std::array<float, 9>{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	const auto triangle_indices = std::array<uint32_t, 3>{
		0, 1, 2
	};

	DeviceFactory factory;
	auto result = factory.create_device(Api::CPU);
	if (result.is_error())
	{
		printf("Error: %s\n", result.error().c_str());
		return 1;
	}

	auto device = result.value();
	printf("Device created: %s\n", device->vendor_id().value());

    auto frame_buffer_result = device->alloc_frame_buffer(PixelFormat::RGBA32SFLOAT, 800, 600);
    if (frame_buffer_result.is_error())
    {
        printf("Error: %s\n", frame_buffer_result.error().c_str());
        return 1;
    }

	auto frame_buffer = frame_buffer_result.value();

	const auto vertex_buffer_result = device->alloc_vertex_buffer(triangle_vertices.data(), triangle_vertices.size());
	if (vertex_buffer_result.is_error())
	{
		printf("Error: %s\n", vertex_buffer_result.error().c_str());
		return 1;
	}

	const auto vertex_buffer = vertex_buffer_result.value();

	const auto index_buffer_result = device->alloc_index_buffer(triangle_indices.data(), triangle_indices.size());
	if (index_buffer_result.is_error())
	{
		printf("Error: %s\n", index_buffer_result.error().c_str());
		return 1;
	}

	const auto index_buffer = index_buffer_result.value();

	const auto acceleration_structure_result = device->alloc_acceleration_structure(vertex_buffer, 3, index_buffer);
	if (acceleration_structure_result.is_error())
	{
		printf("Error: %s\n", acceleration_structure_result.error().c_str());
		return 1;
	}

	const auto acceleration_structure = acceleration_structure_result.value();

	device->build_acceleration_structure(nullptr, acceleration_structure, vertex_buffer, 3, index_buffer);
	return 0;
}