#pragma once
#include "api.hpp"
#include "result.hpp"

class Device;

class DeviceFactory
{
public:
	Result<Device*> create_device(Api api);
};