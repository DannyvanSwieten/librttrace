#pragma once
#include <memory>
class Image
{
public:
	Image(const char* const path);
	Image(const float* data, int width, int height, int channels);

	~Image();

	void write(const char* const path) const;

private:
	struct Pimpl;
	std::unique_ptr<Pimpl> m_pimpl;
};