#include "device.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "frame_buffer.hpp"
#include "bottom_level_acceleration_structure.hpp"

#include <limits.h>
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

CpuDevice::CpuDevice()
{
	CPUID cpu_id(0, 0);
	std::string vendor_id;
	vendor_id += std::string((const char*)&cpu_id.EBX, 4);
	vendor_id += std::string((const char*)&cpu_id.EDX, 4);
	vendor_id += std::string((const char*)&cpu_id.ECX, 4);
	m_vendor_id = vendor_id;
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
CpuDevice::alloc_acceleration_structure(const VertexBuffer* const vertex_buffer, size_t vertex_stride, const IndexBuffer* const index_buffer)
{
	return Result<BottomLevelAccelerationStructure*>::from_value(new CpuBottomLevelAccelerationStructure(vertex_buffer, vertex_stride, index_buffer));
}

Result<Void> CpuDevice::build_acceleration_structure(CommandBuffer* command_buffer,
                                                     BottomLevelAccelerationStructure* acceleration_structure,
                                                     const VertexBuffer* const vertex_buffer,
                                                     size_t vertex_stride,
                                                     const IndexBuffer* const index_buffer)
{
	CpuBottomLevelAccelerationStructure* cpu_acceleration_structure = dynamic_cast<CpuBottomLevelAccelerationStructure*>(acceleration_structure);
	cpu_acceleration_structure->build(vertex_buffer, vertex_stride, index_buffer);
	return Result<Void>::from_value(Void());
}

Result<TopLevelAccelerationStructure*> CpuDevice::alloc_top_level_acceleration_structure(const BottomLevelAccelerationStructure* const acceleration_structures,
                                                                                         size_t count)
{
	return Result<TopLevelAccelerationStructure*>::from_error("not implemented");
}

Result<Void> CpuDevice::build_acceleration_structure(CommandBuffer* command_buffer,
                                                     TopLevelAccelerationStructure* tlas,
                                                     const BottomLevelAccelerationStructure* const acceleration_structures,
                                                     size_t count)
{
	return Result<Void>::from_error("not implemented");
}

Result<FrameBuffer*> CpuDevice::alloc_frame_buffer(PixelFormat format, uint32_t width, uint32_t height)
{
	return Result<FrameBuffer*>::from_value(new CpuFrameBuffer(format, width, height));
}

Result<const char*> CpuDevice::vendor_id() const
{
	return Result<const char*>::from_value(m_vendor_id.c_str());
}
