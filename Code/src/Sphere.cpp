#include "Sphere.h"

#include "Ray.h"
#include "glm/detail/func_geometric.inl"
#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

Sphere::Sphere(const glm::vec3& center, const float radius, std::shared_ptr<Material> mat) : m_radius(std::fmaxf(0.f, radius)), m_center(center), m_mat(mat)
{
	
}

bool Sphere::Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const float directionLength = static_cast<float>(glm::length(ray.GetDirection()));

	const glm::vec3 oc = m_center - ray.GetOrigin();
	const float ocLength = static_cast<float>(glm::length(oc));

	const float a = directionLength * directionLength;
	const float h = glm::dot(oc, ray.GetDirection());
	const float c = ocLength * ocLength - m_radius * m_radius;

	const float discriminant = h * h - a * c;
	if (discriminant < 0)
		return false;

	const float discSqrt = sqrtf(discriminant);

	float root = (h - discSqrt) / a;
	if (!interval.Surrounds(root))
	{
		root = (h + discSqrt) / a;
		if (!interval.Surrounds(root))
			return false;
	}

	rec.t = root;
	rec.p = ray.At(rec.t);
	rec.normal = (rec.p - m_center) / m_radius;
	const glm::vec3 outwardNormal = (rec.p - m_center) / m_radius;
	rec.SetFaceNormal(ray, outwardNormal);
	rec.mat = m_mat;

	return true;
}
