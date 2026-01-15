#pragma once

#include "glm/vec3.hpp"

class Hittable;
class Sphere;

class Ray
{
	public:
	Ray() = default;
	~Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		: m_origin(origin), m_direction(direction) {}
	[[nodiscard]] const glm::vec3& GetOrigin() const { return m_origin; }
	[[nodiscard]] const glm::vec3& GetDirection() const { return m_direction; }

	[[nodiscard]] glm::vec3 At(const float t) const
	{
		return m_origin + t * m_direction;
	}

	glm::vec3 RayColor(const Hittable& world, const int depth);
private:
	glm::vec3 m_origin = glm::vec3(0.0f);
	glm::vec3 m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
};
