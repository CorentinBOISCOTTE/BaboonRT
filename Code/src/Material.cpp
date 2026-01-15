#include "Material.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>

#include "Ray.h"
#include "Hittable.h"
#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

bool Lambertian::Scatter(const Ray& inRay, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	(void)inRay;
	glm::vec3 scatterDirection = rec.normal + RandomUnitVector();
	if (glm::length2(scatterDirection) < 1e-8)
		scatterDirection = rec.normal;
	scattered = Ray(rec.p, scatterDirection);
	attenuation = m_albedo;
	return true;
}

bool Metal::Scatter(const Ray& inRay, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	glm::vec3 reflected = glm::reflect(glm::normalize(inRay.GetDirection()), rec.normal);
	reflected += m_fuzz * RandomUnitVector();
	scattered = Ray(rec.p, reflected);
	attenuation = m_albedo;
	return (glm::dot(scattered.GetDirection(), rec.normal) > 0.f);
}

bool Dielectric::Scatter(const Ray& inRay, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	attenuation = glm::vec3(1.0f);
	float ri = rec.frontFace ? (1.0f / m_refractiveIndex) : m_refractiveIndex;

	glm::vec3 unitDirection = glm::normalize(inRay.GetDirection());

	float cosTheta = glm::min(glm::dot(-unitDirection, rec.normal), 1.0f);
	float sinTheta = glm::sqrt(1.0f - cosTheta * cosTheta);

	bool cannotRefract = ri * sinTheta > 1.0f;
	glm::vec3 direction;

	if (cannotRefract || Reflectance(cosTheta, ri) > RandomFloat())
	{
		direction = glm::reflect(unitDirection, rec.normal);
	}
	else
	{
		direction = glm::refract(unitDirection, rec.normal, ri);
	}

	scattered = Ray(rec.p, direction);
	return true;
}

float Dielectric::Reflectance(const float cosine, const float refIdx) const
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	float r0 = (1.f - refIdx) / (1.f + refIdx);
	r0 *= r0;
	return r0 + (1.f - r0) * glm::pow((1.f - cosine), 5.f);
}
