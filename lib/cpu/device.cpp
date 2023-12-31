#include "device.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "frame_buffer.hpp"
#include "bottom_level_acceleration_structure.hpp"
#include "command_buffer.hpp"
#include "top_level_acceleration_structure.hpp"
#include <limits.h>
#ifdef WIN32
	#include <intrin.h>

struct CPUID
{
	CPUID(int inEAX, int inECX)
	{
		int CPUInfo[4];
		__cpuid(CPUInfo, inEAX);
		EAX = CPUInfo[0];
		EBX = CPUInfo[1];
		ECX = CPUInfo[2];
		EDX = CPUInfo[3];
	}

	int EAX;
	int EBX;
	int ECX;
	int EDX;
};
#endif

CpuDevice::CpuDevice()
{
#ifdef WIN32
	CPUID cpu_id(0, 0);
	std::string vendor_id;
	vendor_id += std::string((const char*)&cpu_id.EBX, 4);
	vendor_id += std::string((const char*)&cpu_id.EDX, 4);
	vendor_id += std::string((const char*)&cpu_id.ECX, 4);
	m_vendor_id = vendor_id;
#endif
}

Result<VertexBuffer*> CpuDevice::alloc_vertex_buffer(const float* const data, size_t size)
{
	return Result<VertexBuffer*>::from_value(new CpuVertexBuffer(data, size));
}

Result<IndexBuffer*> CpuDevice::alloc_index_buffer(const uint32_t* const data, size_t count)
{
	return Result<IndexBuffer*>::from_value(new CpuIndexBuffer(data, count));
}

Result<BottomLevelAccelerationStructure*>
CpuDevice::alloc_bottom_level_acceleration_structure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer)
{
	auto cpu_vertex_buffer = static_cast<const CpuVertexBuffer* const>(vertex_buffer);
	auto cpu_index_buffer = static_cast<const CpuIndexBuffer* const>(index_buffer);
	return Result<BottomLevelAccelerationStructure*>::from_value(new CpuBottomLevelAccelerationStructure(cpu_vertex_buffer, vertex_stride, cpu_index_buffer));
}

Result<TopLevelAccelerationStructure*> CpuDevice::alloc_top_level_acceleration_structure(const Instance* const instances, size_t count)
{
	return Result<TopLevelAccelerationStructure*>::from_value(new CpuTopLevelAccelerationStructure(instances, count));
}

Result<FrameBuffer*> CpuDevice::alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height)
{
	return Result<FrameBuffer*>::from_value(new CpuFrameBuffer(format, width, height));
}

Result<CommandBuffer*> CpuDevice::alloc_command_buffer()
{
	return Result<CommandBuffer*>::from_value(new CpuCommandBuffer());
}

Result<const char*> CpuDevice::vendor_id() const
{
	return Result<const char*>::from_value(m_vendor_id.c_str());
}
