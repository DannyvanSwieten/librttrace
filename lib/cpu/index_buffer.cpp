#include "index_buffer.hpp"

CpuIndexBuffer::CpuIndexBuffer(const uint32_t* const data, size_t count) : m_data(count)
{
	memcpy(m_data.data(), data, count * sizeof(uint32_t));
}