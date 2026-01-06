#pragma once

#include "Ray.h"
#include "Interval.h"
#include "glm/vec3.hpp"

class HitRecord
{
public:
    glm::vec3 p;
    glm::vec3 normal;
    float t;
    bool frontFace;

    void SetFaceNormal(const Ray& r, const glm::vec3& outwardNormal)
	{
        glm::vec3 rayDir = r.GetDirection();
        float dot = rayDir.x * outwardNormal.x +
                    rayDir.y * outwardNormal.y +
					rayDir.z * outwardNormal.z;
        frontFace = dot < 0.f;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& ray, const Interval& interval, HitRecord& rec) const = 0;
};