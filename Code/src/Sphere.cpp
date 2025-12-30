#include "Sphere.h"

#include "Ray.h"
#include "glm/detail/func_geometric.inl"

float Sphere::Hit(const Ray& ray) const
{
	glm::vec3 oc = ray.GetOrigin() - m_center;
	float a = glm::dot(ray.GetDirection(), ray.GetDirection());
	float b = 2.0f * glm::dot(oc, ray.GetDirection());
	float c = glm::dot(oc, oc) - m_radius * m_radius;
	float discriminant = b * b - 4 * a * c;

	return (discriminant < 0) ? -1.0f : (-b - sqrt(discriminant)) / (2.0f * a);
}
