#pragma once

#include "AABB.h"
#include "Hittable.h"
#include <HittableList.h>

class BVHNode : public Hittable
{
public:
	BVHNode(HittableList list);
	BVHNode(std::vector<std::shared_ptr<Hittable>> objects, size_t start, size_t end);
	bool Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override;

private:
	std::shared_ptr<Hittable> m_left;
	std::shared_ptr<Hittable> m_right;

    static bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index);
    static bool BoxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { return BoxCompare(a, b, 0); }
    static bool BoxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { return BoxCompare(a, b, 1); }
    static bool BoxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) { return BoxCompare(a, b, 2); }
};