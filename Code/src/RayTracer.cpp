#include "RayTracer.h"

#include "Camera.h"
#include "HittableList.h"
#include "Ray.h"
#include "ThreadPool.h"

RayTracer::RayTracer(const int samplesPerPixel, int rayDepth) : m_samplesPerPixel(samplesPerPixel), m_rayDepth(rayDepth)
{
	pixelsSampleScale = 1.f / static_cast<float>(samplesPerPixel);
}

void RayTracer::Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer, const Camera* camera, ThreadPool* threadPool, const
                       HittableList& world)
{
	const glm::vec3 pixel00Loc = camera->GetPixel00Loc();
	const glm::vec3 cameraCenter = camera->GetCenter();
	const float pixelWidth = camera->GetViewportWidth() / static_cast<float>(width);
	const float pixelHeight = camera->GetViewportHeight() / static_cast<float>(height);

	//(void)threadPool;
	//for (uint16_t y = 0; y < height; ++y)
	//{
	//	for (uint16_t x = 0; x < width; ++x)
	//	{
	//		float r = 0.f;
	//		float g = 0.f;
	//		float b = 0.f;
	//
	//		for (int i = 0; i < m_samplesPerPixel; ++i)
	//		{
	//			Ray ray = GetRay(x, y, pixel00Loc, cameraCenter, pixelWidth, pixelHeight);
	//			ray.RayColor(world, r, g, b);
	//		}
	//
	//		const float scale = 1.0f / static_cast<float>(m_samplesPerPixel);
	//		r *= scale;
	//		g *= scale;
	//		b *= scale;
	//
	//		//r = std::sqrt(r);
	//		//g = std::sqrt(g);
	//		//b = std::sqrt(b);
	//
	//		float min = 0.f;
	//		float max = 0.999f;
	//
	//		uint8_t red = static_cast<uint8_t>(256.f * Clamp(r, min, max));
	//		uint8_t green = static_cast<uint8_t>(256.f * Clamp(g, min, max));
	//		uint8_t blue = static_cast<uint8_t>(256.f * Clamp(b, min, max));
	//
	//		framebuffer[y * width + x] = RGBA(red, green, blue);
	//	}
	//}

	constexpr int TILE = 16;
	
	for (int ty = 0; ty < height; ty += TILE)
	{
		for (int tx = 0; tx < width; tx += TILE)
		{
			threadPool->Enqueue([ty, height, tx, width, pixel00Loc, pixelWidth, pixelHeight, cameraCenter, &framebuffer, &world, this]()
				{
					for (int y = ty; y < std::min(ty + TILE, static_cast<int>(height)); ++y)
					{
						for (int x = tx; x < std::min(tx + TILE, static_cast<int>(width)); ++x)
						{
							glm::vec3 pixelColor(0.0f);
							
							for (int i = 0; i < m_samplesPerPixel; ++i)
							{
								Ray ray = GetRay(x, y, pixel00Loc, cameraCenter, pixelWidth, pixelHeight);
								pixelColor += ray.RayColor(world, m_rayDepth);
							}
							
							pixelColor /= static_cast<float>(m_samplesPerPixel);
							
							pixelColor.r = std::sqrt(pixelColor.r);
							pixelColor.g = std::sqrt(pixelColor.g);
							pixelColor.b = std::sqrt(pixelColor.b);
							
							const uint8_t r = static_cast<uint8_t>(256.f * Clamp(pixelColor.r, 0.f, 0.999f));
							const uint8_t g = static_cast<uint8_t>(256.f * Clamp(pixelColor.g, 0.f, 0.999f));
							const uint8_t b = static_cast<uint8_t>(256.f * Clamp(pixelColor.b, 0.f, 0.999f));
							
							framebuffer[y * width + x] = RGBA(r, g, b);
						}
					}
				});
		}
	}

	//for (uint16_t y = 0; y < height; ++y)
	//{
	//	threadPool->Enqueue([width, pixel00Loc, pixelWidth, y, pixelHeight, cameraCenter, &world, &framebuffer, this]()
	//		{
	//			for (uint16_t x = 0; x < width; ++x)
	//			{
	//				glm::vec3 pixelColor(0.0f);
	//
	//				for (int i = 0; i < m_samplesPerPixel; ++i)
	//				{
	//					Ray ray = GetRay(x, y, pixel00Loc, cameraCenter, pixelWidth, pixelHeight);
	//					pixelColor += ray.RayColor(world, m_rayDepth);
	//				}
	//
	//				pixelColor /= static_cast<float>(m_samplesPerPixel);
	//
	//				pixelColor.r = std::sqrt(pixelColor.r);
	//				pixelColor.g = std::sqrt(pixelColor.g);
	//				pixelColor.b = std::sqrt(pixelColor.b);
	//
	//				const uint8_t r = static_cast<uint8_t>(256.f * Clamp(pixelColor.r, 0.f, 0.999f));
	//				const uint8_t g = static_cast<uint8_t>(256.f * Clamp(pixelColor.g, 0.f, 0.999f));
	//				const uint8_t b = static_cast<uint8_t>(256.f * Clamp(pixelColor.b, 0.f, 0.999f));
	//
	//				framebuffer[y * width + x] = RGBA(r, g, b);
	//			}
	//		});
	//}

	threadPool->WaitUntilFinished();
}

Ray RayTracer::GetRay(const int x, const int y, const glm::vec3& pixel00Loc, const glm::vec3& cameraCenter, const float pixelWidth, const float pixelHeight)
{
	const glm::vec3 offset = glm::vec3(RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0.f);
	const glm::vec3 pixelSample = pixel00Loc + ((static_cast<float>(x) + offset.x) * glm::vec3(pixelWidth, 0.f, 0.f)) + ((static_cast<float>(y) + offset.y) * glm::vec3(0.f, -pixelHeight, 0.f));

	glm::vec3 rayDirection = pixelSample - cameraCenter;

	return { cameraCenter, rayDirection };
}
