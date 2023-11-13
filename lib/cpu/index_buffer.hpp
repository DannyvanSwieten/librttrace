#pragma once

#include <vector>
#include "../index_buffer.hpp"

class CpuIndexBuffer : public IndexBuffer
{
public:
	CpuIndexBuffer(const uint32_t* const data, size_t count);
	~CpuIndexBuffer() override = default;

	size_t size() const override
	{
		return m_data.size();
	}
	const uint32_t operator[](size_t index) const
	{
		return m_data[index];
	}

private:
	std::vector<uint32_t> m_data;
};