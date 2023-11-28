#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../dependencies/std_image/stb_image.h"
#include "../dependencies/std_image/stb_image_write.h"

struct Image::Pimpl
{
	int m_width;
	int m_height;
	int m_channels;
	float* m_data;

	~Pimpl()
	{
		stbi_image_free(m_data);
	}
};

Image::Image(const char* const path)
{
	m_pimpl = std::make_unique<Pimpl>();
	m_pimpl->m_data = stbi_loadf(path, &m_pimpl->m_width, &m_pimpl->m_height, &m_pimpl->m_channels, 0);
}

Image::Image(const float* data, int width, int height, int channels) : m_pimpl(std::make_unique<Pimpl>())
{
	m_pimpl->m_width = width;
	m_pimpl->m_height = height;
	m_pimpl->m_channels = channels;
	m_pimpl->m_data = new float[width * height * channels];
	memcpy(m_pimpl->m_data, data, width * height * channels * sizeof(float));
}

Image::~Image()
{}

void Image::write(const char* const path) const
{
	const auto result = stbi_write_hdr(path, m_pimpl->m_width, m_pimpl->m_height, m_pimpl->m_channels, m_pimpl->m_data);
	if (result == 0)
	{
		printf("Error: Failed to write image to %s\n", path);
	}
}
