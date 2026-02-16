#include "RayTracer.h"

#include "Camera.h"
#include "HittableList.h"
#include "Ray.h"
#include "ThreadPool.h"
#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

RayTracer::RayTracer(int rayDepth) : m_rayDepth(rayDepth)
{
}

void RayTracer::Render(uint16_t width, uint16_t height, std::vector<Color>& framebuffer, const Camera* camera, ThreadPool* threadPool, const
                       HittableList& world)
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const glm::vec3 pixel00Loc = camera->GetPixel00Loc();
	const glm::vec3 cameraCenter = camera->GetCenter();
	const glm::vec3 horizontalPixelDelta = camera->GetHorizontalPixelDelta();
	const glm::vec3 verticalPixelDelta = camera->GetVerticalPixelDelta();
	const glm::vec3 horizontalDefocusDisk = camera->GetHorizontalDefocusDisk();
	const glm::vec3 verticalDefocusDisk = camera->GetVerticalDefocusDisk();
	const float defocusAngle = camera->GetDefocusAngle();

	const bool sizeChanged = (m_accumWidth != width) || (m_accumHeight != height);
	const bool cameraChanged = (m_accumulatedFrames > 0) && ((cameraCenter != m_lastCameraCenter) || camera->HasRotated());

	if (sizeChanged)
	{
		m_accumWidth = width;
		m_accumHeight = height;
		m_accumBuffer.assign(static_cast<size_t>(width) * static_cast<size_t>(height), glm::vec3(0.0f));
		m_accumulatedFrames = 0;
	}

	if (cameraChanged)
	{
		ResetAccumulation();
	}

	m_lastCameraCenter = cameraCenter;

	constexpr int sppThisFrame = 1;

	//(void)threadPool;
	//for (uint16_t y = 0; y < height; ++y)
	//{
	//	for (uint16_t x = 0; x < width; ++x)
	//	{
	//		glm::vec3 pixelColor(0.0f);
	//
	//		for (int i = 0; i < sppThisFrame; ++i)
	//		{
	//			Ray ray = GetRay(x, y, pixel00Loc, cameraCenter, pixelWidth, pixelHeight);
	//			pixelColor += ray.RayColor(world, m_rayDepth);
	//		}
	//
	//		pixelColor /= static_cast<float>(sppThisFrame);
	//
	//		const size_t idx = static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x);
	//		m_accumBuffer[idx] += pixelColor;
	//
	//		const float inv = 1.0f / static_cast<float>(m_accumulatedFrames + 1);
	//		glm::vec3 avg = m_accumBuffer[idx] * inv;
	//
	//		pixelColor.r = std::sqrt(avg.r);
	//		pixelColor.g = std::sqrt(avg.g);
	//		pixelColor.b = std::sqrt(avg.b);
	//
	//		const uint8_t r = static_cast<uint8_t>(256.f * Clamp(pixelColor.r, 0.f, 0.999f));
	//		const uint8_t g = static_cast<uint8_t>(256.f * Clamp(pixelColor.g, 0.f, 0.999f));
	//		const uint8_t b = static_cast<uint8_t>(256.f * Clamp(pixelColor.b, 0.f, 0.999f));
	//
	//		framebuffer[y * width + x] = RGBA(r, g, b);
	//	}
	//}

	constexpr int TILE = 256;
	
	for (int ty = 0; ty < height; ty += TILE)
	{
		for (int tx = 0; tx < width; tx += TILE)
		{
			threadPool->Enqueue(
				[ty, height, tx, width, pixel00Loc, horizontalPixelDelta, verticalPixelDelta, horizontalDefocusDisk,
					verticalDefocusDisk, defocusAngle, cameraCenter, &framebuffer, &world, this]()
				{
#ifdef TRACY_ENABLE
					ZoneScoped
#endif
					for (int y = ty; y < std::min(ty + TILE, static_cast<int>(height)); ++y)
					{
						for (int x = tx; x < std::min(tx + TILE, static_cast<int>(width)); ++x)
						{
							glm::vec3 pixelColor(0.0f);
							
							for (int i = 0; i < sppThisFrame; ++i)
							{
								Ray ray = GetRay(x, y, pixel00Loc, cameraCenter, horizontalPixelDelta, verticalPixelDelta, horizontalDefocusDisk, verticalDefocusDisk, defocusAngle);
								pixelColor += ray.RayColor(world, m_rayDepth);
							}
							
							pixelColor /= static_cast<float>(sppThisFrame);
	
							const size_t idx = static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x);
							m_accumBuffer[idx] += pixelColor;
	
							const float inv = 1.0f / static_cast<float>(m_accumulatedFrames + 1);
							glm::vec3 avg = m_accumBuffer[idx] * inv;
							
							pixelColor.r = std::sqrt(avg.r);
							pixelColor.g = std::sqrt(avg.g);
							pixelColor.b = std::sqrt(avg.b);
							
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
	//				for (int i = 0; i < sppThisFrame; ++i)
	//				{
	//					Ray ray = GetRay(x, y, pixel00Loc, cameraCenter, pixelWidth, pixelHeight);
	//					pixelColor += ray.RayColor(world, m_rayDepth);
	//				}
	//
	//				pixelColor /= static_cast<float>(sppThisFrame);
	//
	//				const size_t idx = static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x);
	//				m_accumBuffer[idx] += pixelColor;
	//
	//				const float inv = 1.0f / static_cast<float>(m_accumulatedFrames + 1);
	//				glm::vec3 avg = m_accumBuffer[idx] * inv;
	//
	//				pixelColor.r = std::sqrt(avg.r);
	//				pixelColor.g = std::sqrt(avg.g);
	//				pixelColor.b = std::sqrt(avg.b);
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
	++m_accumulatedFrames;
}

Ray RayTracer::GetRay(const int x, const int y, const glm::vec3& pixel00Loc, const glm::vec3& cameraCenter,
                      const glm::vec3& horizontalPixelDelta, const glm::vec3& verticalPixelDelta,
                      const glm::vec3& horizontalDefocusDisk, const glm::vec3& verticalDefocusDisk, const float defocusAngle)
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const glm::vec3 offset = glm::vec3(RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0.f);
	const float sx = static_cast<float>(x) + offset.x;
	const float sy = static_cast<float>(y) + offset.y;

	const glm::vec3 pixelSample = pixel00Loc + (sx * horizontalPixelDelta) - (sy * verticalPixelDelta);
	const glm::vec3 rayOrigin = (defocusAngle <= 0.f) ? cameraCenter : DefocusDiskSample(cameraCenter, horizontalDefocusDisk, verticalDefocusDisk);
	const glm::vec3 rayDirection = pixelSample - rayOrigin;

	return { cameraCenter, rayDirection };
}

glm::vec3 RayTracer::DefocusDiskSample(const glm::vec3& cameraCenter, const glm::vec3& horizontalDefocusDisk, const glm::vec3& verticalDefocusDisk)
{
	const glm::vec3 p = RandomInUnitDisk();
	return (p.x * horizontalDefocusDisk) + (p.y * verticalDefocusDisk) + cameraCenter;
}

void RayTracer::ResetAccumulation()
{
	m_accumulatedFrames = 0;
	std::ranges::fill(m_accumBuffer, glm::vec3(0.0f));
}
