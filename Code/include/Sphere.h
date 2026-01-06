#pragma once
#include "Hittable.h"
#include "glm/vec3.hpp"

class Ray;

class Sphere : public Hittable
{
	public:
	Sphere() = default;
	Sphere(const glm::vec3& center, const float radius)
		: m_radius(std::fmaxf(0.f, radius)), m_center(center) {}

	[[nodiscard]] float GetRadius() const { return m_radius; }
	[[nodiscard]] glm::vec3 GetCenter() const { return m_center; }

	bool Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override;

	void SetPosition(const glm::vec3& newPos) { m_center = newPos; }

private:
	float m_radius = 1.0f;
	glm::vec3 m_center = glm::vec3(0.0f);
};
