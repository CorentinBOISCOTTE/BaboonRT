#include "RayTracer.h"

void RayTracer::Render(uint16_t width, uint16_t height, std::vector<uint32_t>& framebuffer)
{
	for (uint16_t y = 0; y < height; ++y)
	{
		for (uint16_t x = 0; x < width; ++x)
		{
			uint8_t r = static_cast<uint8_t>((float(x) / width) * 255);
			uint8_t g = static_cast<uint8_t>((float(y) / height) * 255);
			uint8_t b = 128;
			framebuffer[y * width + x] = RGBA(r, g, b);
		}
	}
}
