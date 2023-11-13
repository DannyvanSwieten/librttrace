#pragma once
#include "result.hpp"
class FrameBuffer
{
public:
	virtual ~FrameBuffer() = 0;

	virtual void set_pixel(uint32_t x, uint32_t y, float r, float g, float b, float a) = 0;
	virtual void clear() = 0;

	virtual Result<const void*> data() = 0;
	virtual uint32_t width() = 0;
	virtual uint32_t height() = 0;
};
