#pragma once

#include <memory>

#include "Hittable.h"

#include <vector>

class HittableList : public Hittable
{
	public:
	HittableList() = default;
	HittableList(std::shared_ptr<Hittable> object) { Add(object); }
	void Clear() { m_objects.clear(); }
	void Add(const std::shared_ptr<Hittable>& object) { m_objects.push_back(object); m_bbox = AABB(m_bbox, object->BoundingBox()); m_size += 1; }
	bool Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override;

	AABB BoundingBox() const override { return m_bbox; }
	std::vector<std::shared_ptr<Hittable>> GetObjects() const { return m_objects; }
	size_t GetSize() const { return m_size; }

private:
	std::vector<std::shared_ptr<Hittable>> m_objects;
	AABB m_bbox;
	size_t m_size;
};