#pragma once
#include <limits>
#include <random>

#include <glm/vec3.hpp>

constexpr float infinity = std::numeric_limits<float>::infinity();

inline float RandomFloat()
{
    thread_local std::mt19937 gen{ std::random_device{}() };
    thread_local std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

inline float RandomFloat(const float min, const float max)
{
    return min + (max - min) * RandomFloat();
}

inline float Clamp(const float value, const float min, const float max)
{
	return std::min(std::max(value, min), max);
}

inline float Length2(const glm::vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline glm::vec3 RandomUnitVector()
{
    constexpr float kMinLenSqr = std::numeric_limits<float>::min();

    while (true)
    {
        const glm::vec3 p = glm::vec3(
            RandomFloat(-1.f, 1.f),
            RandomFloat(-1.f, 1.f),
            RandomFloat(-1.f, 1.f)
        );

        const float lenSqr = Length2(p);
        if (lenSqr > 1.f)
            continue;

        if (lenSqr < kMinLenSqr)
            continue;

        return p / std::sqrt(lenSqr);
    }
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal)
{
    const glm::vec3 inUnitSphere = RandomUnitVector();
	const float dot = inUnitSphere.x * normal.x + inUnitSphere.y * normal.y + inUnitSphere.z * normal.z;

    if (dot > 0.f)
    {
        return inUnitSphere;
    }

	return -inUnitSphere;
}
