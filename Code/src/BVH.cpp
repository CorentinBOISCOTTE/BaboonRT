#include "BVH.h"

BVHNode::BVHNode(HittableList list) : BVHNode(list.GetObjects(), 0, list.GetSize())
{
}

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end)
{
}

bool BVHNode::Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const
{
	if (!m_bbox.Hit(ray, interval))
		return false;

	const bool hitLeft = m_left->Hit(ray, interval, rec);
	const bool hitRight = m_right->Hit(ray, interval, rec);

	return hitLeft || hitRight;
}
