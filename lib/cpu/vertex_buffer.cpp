#include "vertex_buffer.hpp"

CpuVertexBuffer::CpuVertexBuffer(const float* const data, size_t size) : m_data(size)
{
	memcpy(m_data.data(), data, size);
}

CpuVertexBuffer::~CpuVertexBuffer()
{}

const float CpuVertexBuffer::operator[](size_t index) const
{
	return m_data[index];
}
