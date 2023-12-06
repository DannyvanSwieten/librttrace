#pragma once
#include <cstddef>
class IndexBuffer
{
public:
	virtual ~IndexBuffer() = 0;
	virtual size_t size() const = 0;
};
