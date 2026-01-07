#pragma once
#include "Hittable.h"

class Ray;

class Sphere : public Hittable
{
	public:
	Sphere() = default;
	Sphere(const glm::vec3& center, const float radius, std::shared_ptr<Material> mat);

	[[nodiscard]] float GetRadius() const { return m_radius; }
	[[nodiscard]] glm::vec3 GetCenter() const { return m_center; }

	bool Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override;

	void SetPosition(const glm::vec3& newPos) { m_center = newPos; }

private:
	float m_radius = 1.0f;
	glm::vec3 m_center = glm::vec3(0.0f);
	std::shared_ptr<Material> m_mat;
};
