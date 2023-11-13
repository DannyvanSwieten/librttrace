#include "device_factory.hpp"
#include "cpu/device.hpp"
#include "vulkan/device.hpp"

Result<Device*> DeviceFactory::create_device(Api api)

{
	switch (api)
	{
		case Api::METAL:
		{
#ifdef WIN32
			return Result<Device*>::from_error("You're on windows mate. No metal found here :(");
#endif
		}
		case Api::CPU:
		{
			return Result<Device*>::from_value(new CpuDevice());
		}
        case Api::VULKAN:
        {
            return Result<Device*>::from_value(new VulkanDevice());
        }
	}

	return Result<Device*>::from_error("Unknown API");
}