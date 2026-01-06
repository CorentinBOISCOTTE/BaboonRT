#pragma once

#include <memory>

#include "Hittable.h"

#include <vector>

class HittableList : public Hittable
{
	public:
	HittableList() = default;
	HittableList(const std::vector<std::shared_ptr<Hittable>>& objects)
		: m_objects(objects) {}
	void Clear() { m_objects.clear(); }
	void Add(const std::shared_ptr<Hittable>& object) { m_objects.push_back(object); }
	bool Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const override;

private:
	std::vector<std::shared_ptr<Hittable>> m_objects;
};