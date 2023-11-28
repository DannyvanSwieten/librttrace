#include "frame_buffer.hpp"

CpuFrameBuffer::CpuFrameBuffer(PixelFormat format, uint32_t width, uint32_t height)
	: m_width(width)
	, m_height(height)
	, m_bytes_per_pixel(format == PixelFormat::RGBA8UINT ? 4 : 16)
	, m_data(width * height * m_bytes_per_pixel)
{
	m_data.resize(width * height * m_bytes_per_pixel);
}

void CpuFrameBuffer::set_pixel(uint32_t x, uint32_t y, float r, float g, float b, float a)
{
	size_t offset = (x + y * m_width) * 4;
	if (m_bytes_per_pixel == 16)
	{
		auto address = reinterpret_cast<float*>(m_data.data()) + offset;
		address[0] = r;
		address[1] = g;
		address[2] = b;
		address[3] = a;
	}
	else
	{
		m_data[offset + 0] = static_cast<char>(r * 255);
		m_data[offset + 1] = static_cast<char>(g * 255);
		m_data[offset + 2] = static_cast<char>(b * 255);
		m_data[offset + 3] = static_cast<char>(a * 255);
	}
}

void CpuFrameBuffer::clear()
{
	memset(m_data.data(), 0, m_data.size());
}

Result<const void*> CpuFrameBuffer::data()
{
	return Result<const void*>::from_value(m_data.data());
}

uint32_t CpuFrameBuffer::width()
{
	return m_width;
}

uint32_t CpuFrameBuffer::height()
{
	return m_height;
}