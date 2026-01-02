#include "RayTracer.h"

#include "Camera.h"
#include "Ray.h"
#include "ThreadPool.h"

void RayTracer::Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer, const Camera* camera, ThreadPool* threadPool, const
                       std::vector<Sphere>& spheres)
{
	const glm::vec3 pixel00Loc = camera->GetPixel00Loc();
	const glm::vec3 cameraCenter = camera->GetCenter();
	const float pixelWidth = camera->GetViewportWidth() / static_cast<float>(width);
	const float pixelHeight = camera->GetViewportHeight() / static_cast<float>(height);

	//for (uint16_t y = 0; y < height; ++y)
	//{
	//	for (uint16_t x = 0; x < width; ++x)
	//	{
	//		glm::vec3 pixelCenter = pixel00Loc +
	//			glm::vec3((static_cast<float>(x) + 0.5f) * pixelWidth,
	//				(static_cast<float>(y) + 0.5f) * pixelHeight,
	//				0.f);
	//
	//		glm::vec3 rayDirection = pixelCenter - cameraCenter;
	//
	//		Ray ray(cameraCenter, rayDirection);
	//
	//		framebuffer[y * width + x] = ray.RayColor(m_spheres);
	//	}
	//}

	//constexpr int TILE = 16;
	//
	//for (int ty = 0; ty < height; ty += TILE)
	//{
	//	for (int tx = 0; tx < width; tx += TILE)
	//	{
	//		threadPool->Enqueue([ty, height, tx, width, pixel00Loc, pixelWidth, pixelHeight, cameraCenter, &framebuffer, spheres]()
	//			{
	//				for (int y = ty; y < std::min(ty + TILE, static_cast<int>(height)); ++y)
	//				{
	//					for (int x = tx; x < std::min(tx + TILE, static_cast<int>(width)); ++x)
	//					{
	//						glm::vec3 pixelCenter = pixel00Loc +
	//							glm::vec3((static_cast<float>(x) + 0.5f) * pixelWidth,
	//								(static_cast<float>(y) + 0.5f) * pixelHeight,
	//								0.f);
	//
	//						glm::vec3 rayDirection = pixelCenter - cameraCenter;
	//
	//						Ray ray(cameraCenter, rayDirection);
	//
	//						framebuffer[y * width + x] = ray.RayColor(spheres);
	//					}
	//				}
	//			});
	//	}
	//}

	for (uint16_t y = 0; y < height; ++y)
	{
		threadPool->Enqueue([width, pixel00Loc, pixelWidth, y, pixelHeight, cameraCenter, spheres, &framebuffer]()
			{
				for (uint16_t x = 0; x < width; ++x)
				{
					glm::vec3 pixelCenter = pixel00Loc +
						glm::vec3((static_cast<float>(x) + 0.5f) * pixelWidth,
							(static_cast<float>(y) + 0.5f) * pixelHeight,
							0.f);
	
					glm::vec3 rayDirection = pixelCenter - cameraCenter;
	
					Ray ray(cameraCenter, rayDirection);
	
					framebuffer[y * width + x] = ray.RayColor(spheres);
				}
			});
	}
}
