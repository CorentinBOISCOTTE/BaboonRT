#pragma once
#include "glm/vec3.hpp"

class Ray;

class Sphere
{
	public:
	Sphere() = default;
	~Sphere() = default;
	Sphere(const float radius, const glm::vec3& center)
		: m_radius(radius), m_center(center) {}

	[[nodiscard]] float GetRadius() const { return m_radius; }
	[[nodiscard]] glm::vec3 GetCenter() const { return m_center; }

	float Hit(const Ray& ray) const;

	void SetPosition(const glm::vec3& newPos) { m_center = newPos; }

private:
	float m_radius = 1.0f;
	glm::vec3 m_center = glm::vec3(0.0f);
};
