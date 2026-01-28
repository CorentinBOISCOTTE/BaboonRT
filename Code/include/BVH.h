#pragma once

#include "AABB.h"
#include "Hittable.h"
#include <HittableList.h>

class BVHNode : public Hittable
{
	BVHNode(HittableList list);
	BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);
	bool Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override;
	AABB BoundingBox() const override { return m_bbox; }
private:
	std::shared_ptr<Hittable> m_left;
	std::shared_ptr<Hittable> m_right;
	AABB m_bbox;
};