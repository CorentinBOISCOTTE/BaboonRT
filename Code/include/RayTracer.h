#pragma once
#include <cstdint>
#include <vector>

#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"

class ThreadPool;
class Camera;

class RayTracer
{
public:
	RayTracer(int samplesPerPixel);
	~RayTracer() = default;
	void Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer, const Camera* camera, ThreadPool* threadPool, HittableList
	            world);

private:
	int m_samplesPerPixel;
	float pixelsSampleScale;


	Ray GetRay(const int x, const int y, const glm::vec3& pixel00Loc, const glm::vec3& cameraCenter, const float pixelWidth, const float pixelHeight);
};
