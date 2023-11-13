#pragma once
#include "../vertex_buffer.hpp"
#include <vector>

class CpuVertexBuffer : public VertexBuffer
{
public:
	CpuVertexBuffer(const float* const data, size_t size);
	~CpuVertexBuffer() override;

	const float operator[](size_t index) const;
	size_t size() const override
	{
		return m_data.size();
	}

	const float* data() const
	{
		return m_data.data();
	}

private:
	std::vector<float> m_data;
};