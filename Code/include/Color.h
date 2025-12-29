#pragma once
#include <cstdint>

using Color = uint32_t;

inline Color RGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255)
{
    return (a << 24) | (b << 16) | (g << 8) | r;
}

inline uint8_t GetR(const uint32_t c) { return c & 0xFF; }
inline uint8_t GetG(const uint32_t c) { return (c >> 8) & 0xFF; }
inline uint8_t GetB(const uint32_t c) { return (c >> 16) & 0xFF; }
inline uint8_t GetA(const uint32_t c) { return (c >> 24) & 0xFF; }