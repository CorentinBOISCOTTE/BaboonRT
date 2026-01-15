#include "HittableList.h"
#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

bool HittableList::Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	HitRecord tempRec;
	bool hitAnything = false;
	float closestSoFar = interval.max;
	for (const auto& object : m_objects)
	{
		if (object->Hit(ray, Interval(interval.min, closestSoFar), tempRec))
		{
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}
