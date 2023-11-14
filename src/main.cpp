#include <device_factory.hpp>
#include <device.hpp>

int main()
{
	DeviceFactory factory;
	auto result = factory.create_device(Api::VULKAN);
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

	return 0;
}