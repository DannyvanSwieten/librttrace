#pragma once
#include "../float3.hpp"
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

	const Float3 float3(size_t index) const
	{
		float x = m_data[index];
		float y = m_data[index + 1];
		float z = m_data[index + 2];
		return Float3(x, y, z);
	}

private:
	std::vector<float> m_data;
};