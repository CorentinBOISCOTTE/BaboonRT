#pragma once
#include "Utils.h"
#include "glm/common.hpp"

class Interval
{
public:
    float min, max;

    Interval() : min(+infinity), max(-infinity) {}

    Interval(const float min, const float max) : min(min), max(max) {}

    [[nodiscard]] float Size() const
	{
        return max - min;
    }

    [[nodiscard]] bool Contains(const float x) const
	{
        return min <= x && x <= max;
    }

    [[nodiscard]] bool Surrounds(const float x) const
	{
        return min < x && x < max;
    }

	[[nodiscard]] float Clamp(float x) const
    {
    	return glm::clamp(x, min, max);
    }

	Interval Expand(const float delta) const
    {
		const float padding = delta * 0.5f;
        return { min - padding, max + padding };
    }
};
