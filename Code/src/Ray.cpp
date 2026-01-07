#include "Ray.h"

#include "Color.h"
#include "Sphere.h"
#include "Material.h"
#include "glm/detail/func_geometric.inl"

glm::vec3 Ray::RayColor(const Hittable& world, const int depth)
{
	if (depth <= 0)
		return glm::vec3(0.f);

    HitRecord rec;

    if (world.Hit(*this, Interval(0.001f, infinity), rec))
    {
        Ray scattered;
        glm::vec3 attenuation;
        if (rec.mat->Scatter(*this, rec, attenuation, scattered))
            return attenuation * scattered.RayColor(world, depth - 1);
        return {0, 0, 0};
    }

    const glm::vec3 unitDir = glm::normalize(m_direction);
    const float t = 0.5f * (unitDir.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}
