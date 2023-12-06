#pragma once
#include <cstddef>
class VertexBuffer
{
public:
	virtual ~VertexBuffer() = 0;
	virtual size_t size() const = 0;
};