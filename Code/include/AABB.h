#pragma once
#include "Interval.h"

class Ray;

class AABB
{
public:
	Interval x, y, z;
	AABB() : x(), y(), z() {}
	AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {}
	AABB(const glm::vec3& min, const glm::vec3& max);
	AABB(const AABB& a, const AABB& b)
	{
		x = Interval(glm::min(a.x.min, b.x.min), glm::max(a.x.max, b.x.max));
		y = Interval(glm::min(a.y.min, b.y.min), glm::max(a.y.max, b.y.max));
		z = Interval(glm::min(a.z.min, b.z.min), glm::max(a.z.max, b.z.max));
	}

	const Interval& operator[](const int index) const;

	[[nodiscard]] bool Hit(const Ray& ray, Interval t) const;

	[[nodiscard]] glm::vec3 Min() const { return {x.min, y.min, z.min}; }
	[[nodiscard]] glm::vec3 Max() const { return {x.max, y.max, z.max}; }
};
