#pragma once
#include <limits>
#include <random>

#include <glm/vec3.hpp>

#include "glm/ext/scalar_constants.hpp"

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
    const float z = RandomFloat(-1.0f, 1.0f);
    const float a = RandomFloat(0.0f, 2.0f * glm::pi<float>());

    const float r = std::sqrt(1.0f - z * z);
    return {
	    r * std::cos(a),
        r * std::sin(a),
        z
    };
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
