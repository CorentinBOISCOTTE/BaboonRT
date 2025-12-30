#include "Ray.h"

#include "Sphere.h"
#include "glm/detail/func_geometric.inl"

Color Ray::RayColor(const std::vector<Sphere>& spheres)
{

	for (const Sphere& sphere : spheres)
	{
		float t = sphere.Hit(*this);
		if (t > 0.0f)
		{
			glm::vec3 normal = glm::normalize(At(t) - sphere.GetCenter());
			uint8_t r = static_cast<uint8_t>(0.5 * (normal.x + 1.0) * 255);
			uint8_t g = static_cast<uint8_t>(0.5 * (normal.y + 1.0) * 255);
			uint8_t b = static_cast<uint8_t>(0.5 * (normal.z + 1.0) * 255);
			return RGBA(r, g, b);
		}
	}

    glm::vec3 normalizedDir = glm::normalize(m_direction);
    auto a = 0.5 * (normalizedDir.y + 1.0);
	const uint8_t r = static_cast<uint8_t>((1.0 - a) * 255 + a * 128);
	const uint8_t g = static_cast<uint8_t>((1.0 - a) * 255 + a * 179);
	const uint8_t b = static_cast<uint8_t>((1.0 - a) * 255 + a * 255);
	return RGBA(r, g, b);
}
