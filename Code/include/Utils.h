#pragma once
#include <limits>
#include <random>

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
