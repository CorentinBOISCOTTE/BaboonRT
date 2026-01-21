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
	RayTracer(int rayDepth);
	~RayTracer() = default;
	void Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer, const Camera* camera, ThreadPool* threadPool, const HittableList
	            & world);

private:
	int m_rayDepth;

	std::vector<glm::vec3> m_accumBuffer;
	uint64_t m_accumulatedFrames = 0;
	uint16_t m_accumWidth = 0;
	uint16_t m_accumHeight = 0;
	glm::vec3 m_lastCameraCenter = glm::vec3(0.0f);

	Ray GetRay(const int x, const int y, const glm::vec3& pixel00Loc, const glm::vec3& cameraCenter,
	           const glm::vec3& horizontalPixelDelta, const glm::vec3& verticalPixelDelta,
	           const glm::vec3& horizontalDefocusDisk, const glm::vec3& verticalDefocusDisk, const float defocusAngle);
	static glm::vec3 DefocusDiskSample(const glm::vec3& cameraCenter, const glm::vec3& horizontalDefocusDisk, const glm::vec3& verticalDefocusDisk);
	void ResetAccumulation();
};
