#include "BVH.h"

BVHNode::BVHNode(HittableList list) : BVHNode(list.GetObjects(), 0, list.GetSize())
{
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>> objects, size_t start, size_t end)
{
	m_bbox = AABB::empty;

	for (size_t objIndex = start; objIndex < end; ++objIndex)
		m_bbox = AABB(m_bbox, objects[objIndex]->BoundingBox());

	int axis = m_bbox.LongestAxis();

	auto comparator = (axis == 0) ? BoxXCompare
					: (axis == 1) ? BoxYCompare
					: BoxZCompare;

	size_t objectSpan = end - start;

	if (objectSpan == 1)
		m_left = m_right = objects[start];
	else if (objectSpan == 2)
	{
		m_left = objects[start];
		m_right = objects[start + 1];
	}
	else
	{
		std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

		size_t mid = start + objectSpan / 2;
		m_left = std::make_shared<BVHNode>(objects, start, mid);
		m_right = std::make_shared<BVHNode>(objects, mid, end);
	}
}

bool BVHNode::Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const
{
	if (!m_bbox.Hit(ray, interval))
		return false;

	const bool hitLeft = m_left->Hit(ray, interval, rec);
	const bool hitRight = m_right->Hit(ray, interval, rec);

	return hitLeft || hitRight;
}

bool BVHNode::BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index)
{
	Interval aAxisInterval = a->BoundingBox()[axis_index];
	Interval bAxisInterval = b->BoundingBox()[axis_index];
	return aAxisInterval.min < bAxisInterval.min;
}