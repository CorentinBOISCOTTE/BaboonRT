#pragma once

#include "glm/vec3.hpp"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;
	Camera(const uint16_t imageWidth, const uint16_t imageHeight, const float viewportHeight, const glm::vec3& center, const float focalLength = 1.f, const float vFov = 90.f);

	[[nodiscard]] const float& GetViewportWidth() const { return m_viewportWidth; }
	[[nodiscard]] const float& GetViewportHeight() const { return m_viewportHeight; }
	[[nodiscard]] const float& GetFocalLength() const { return m_focalLength; }
	[[nodiscard]] const glm::vec3& GetCenter() const { return m_center; }
	[[nodiscard]] const glm::vec3& GetPixel00Loc() const { return m_pixel00Loc; }

	void Move(const glm::vec3& delta) { m_center += delta; m_pixel00Loc += delta; }

private:
	float m_viewportWidth, m_viewportHeight;
	float m_focalLength = 1.f;
	float m_fov = 90.f;
	glm::vec3 m_center;
	glm::vec3 m_pixel00Loc;
};