#include "RayTracer.h"

#include "Camera.h"
#include "Ray.h"

void RayTracer::Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer, const Camera* camera)
{
	for (uint16_t y = 0; y < height; ++y)
	{
		for (uint16_t x = 0; x < width; ++x)
		{
			glm::vec3 pixelCenter = camera->GetPixel00Loc() +
				glm::vec3((static_cast<float>(x) + 0.5f) * (camera->GetViewportWidth() / static_cast<float>(width)),
					(static_cast<float>(y) + 0.5f) * (camera->GetViewportHeight() / static_cast<float>(height)),
					0.f);

			glm::vec3 rayDirection = pixelCenter - camera->GetCenter();

			Ray ray(camera->GetCenter(), rayDirection);

			framebuffer[y * width + x] = ray.RayColor(m_spheres);
		}
	}
}
