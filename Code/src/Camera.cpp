#include "Camera.h"

#include "glm/detail/func_trigonometric.inl"

Camera::Camera(const uint16_t imageWidth, const uint16_t imageHeight, const float viewportHeight,
	const glm::vec3& center, const float focalLength, const float vFov) : m_viewportHeight(viewportHeight), m_focalLength(focalLength), m_fov(vFov), m_center(center)
{
	float theta = glm::radians(m_fov);
	float h = glm::tan(theta / 2.f);
	m_viewportHeight = 2.f * h * focalLength;
	m_viewportWidth = (static_cast<float>(imageWidth) / static_cast<float>(imageHeight)) * m_viewportHeight;
	m_pixel00Loc = m_center
		- glm::vec3(m_viewportWidth / 2.f, 0.f, 0.f)
		+ glm::vec3(0.f, m_viewportHeight / 2.f, 0.f)
		- glm::vec3(0.f, 0.f, m_focalLength);
}
