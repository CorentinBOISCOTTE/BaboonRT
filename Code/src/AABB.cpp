#include "AABB.h"

#include "Ray.h"
#include "spdlog/spdlog.h"

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
{
	x = (min.x <= max.x) ? Interval(min.x, max.x) : Interval(max.x, min.x);
	y = (min.y <= max.y) ? Interval(min.y, max.y) : Interval(max.y, min.y);
	z = (min.z <= max.z) ? Interval(min.z, max.z) : Interval(max.z, min.z);
}

const Interval& AABB::operator[](const int index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		spdlog::error("AABB index out of bounds: {}", index);
	}
	return x;
}

bool AABB::Hit(const Ray& ray, Interval t) const
{
	const glm::vec3 origin = ray.GetOrigin();
	const glm::vec3 direction = ray.GetDirection();

	for (int axis = 0; axis < 3; ++axis)
	{
		const Interval& ax = (*this)[axis];
		const float invD = 1.0f / direction[axis];

		const float t0 = (ax.min - origin[axis]) * invD;
		const float t1 = (ax.max - origin[axis]) * invD;

		if (t0 < t1)
		{
			t.min = t0 > t.min ? t0 : t.min;
			t.max = t1 < t.max ? t1 : t.max;
		}
		else
		{
			t.min = t1 > t.min ? t1 : t.min;
			t.max = t0 < t.max ? t0 : t.max;
		}

		if (t.max <= t.min)
			return false;
	}
	return true;
}
