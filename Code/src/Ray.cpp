#include "Ray.h"

#include "Sphere.h"
#include "glm/detail/func_geometric.inl"

glm::vec3 Ray::RayColor(const Hittable& world, const int depth)
{
	if (depth <= 0)
		return glm::vec3(0.f);

    HitRecord rec;

    if (world.Hit(*this, Interval(0.001f, infinity), rec))
    {
        glm::vec3 target = rec.normal + RandomOnHemisphere(rec.normal);
        Ray scattered(rec.p, target);
        glm::vec3 attenuation(0.3f);
        return attenuation * scattered.RayColor(world, depth - 1);
    }

    glm::vec3 unitDir = glm::normalize(m_direction);
    float t = 0.5f * (unitDir.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}
