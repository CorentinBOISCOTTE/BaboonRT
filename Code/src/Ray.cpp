#include "Ray.h"

#include "Sphere.h"
#include "glm/detail/func_geometric.inl"

void Ray::RayColor(const Hittable& world, float& r, float& g, float& b)
{
	HitRecord rec;

	if (world.Hit(*this, Interval(0, infinity), rec)) 
	{
		const glm::vec3 normal = rec.normal;
		r += 0.5f * (normal.x + 1.0f);
		g += 0.5f * (normal.y + 1.f);
		b += 0.5f * (normal.z + 1.f);
	}
	else
	{
		glm::vec3 normalizedDir = glm::normalize(m_direction);
		const float a = 0.5f * (normalizedDir.y + 1.f);
		r += (1.f - a) + a * 0.5f;
		g += (1.f - a) + a * 0.7f;
		b += (1.f - a) + a * 1.f;
	}
}
