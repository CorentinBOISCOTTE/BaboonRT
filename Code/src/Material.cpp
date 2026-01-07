#include "Material.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>

#include "Ray.h"
#include "Hittable.h"

bool Lambertian::Scatter(const Ray& inRay, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
{
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
	glm::vec3 reflected = glm::reflect(glm::normalize(inRay.GetDirection()), rec.normal);
	reflected += m_fuzz * RandomUnitVector();
	scattered = Ray(rec.p, reflected);
	attenuation = m_albedo;
	return (glm::dot(scattered.GetDirection(), rec.normal) > 0.f);
}
