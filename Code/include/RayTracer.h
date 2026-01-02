#pragma once
#include <cstdint>
#include <vector>

#include "Color.h"
#include "Sphere.h"

class ThreadPool;
class Camera;

class RayTracer
{
public:
	RayTracer() = default;
	~RayTracer() = default;
	void Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer, const Camera* camera, ThreadPool* threadPool, const std::vector<Sphere>& spheres);
};
