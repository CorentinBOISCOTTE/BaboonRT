#include "Sphere.h"

#include "Ray.h"
#include "glm/detail/func_geometric.inl"

bool Sphere::Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const
{
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

	return true;
}
