#pragma once
#include <cstdint>
#include <vector>

#include "Color.h"
#include "Sphere.h"

class RayTracer
{
public:
	RayTracer() = default;
	~RayTracer() = default;
	explicit RayTracer(const std::vector<Sphere>& spheres) : m_spheres(spheres) {}
	void Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer);

private:
	std::vector<Sphere> m_spheres;
};
