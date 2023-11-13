#pragma once
#include <vector>
#include "../pixel_format.hpp"
#include "../frame_buffer.hpp"

class CpuFrameBuffer : public FrameBuffer
{
public:
	CpuFrameBuffer(PixelFormat format, uint32_t width, uint32_t height);
	void set_pixel(uint32_t x, uint32_t y, float r, float g, float b, float a) override;
	void clear() override;

	Result<const void*> data() override;
	uint32_t width() override;
	uint32_t height() override;

private:
	uint32_t m_width;
	uint32_t m_height;
	size_t m_bytes_per_pixel;
	std::vector<char> m_data;
};