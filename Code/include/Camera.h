#pragma once

#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;
	Camera(const uint16_t imageWidth, const uint16_t imageHeight, const float viewportHeight, const glm::vec3& center,
	       const glm::vec3& lookAt, const glm::vec3& up, const float vFov = 90.f);

	[[nodiscard]] const float& GetViewportWidth() const { return m_viewportWidth; }
	[[nodiscard]] const float& GetViewportHeight() const { return m_viewportHeight; }
	[[nodiscard]] const float& GetFocalLength() const { return m_focalLength; }
	[[nodiscard]] const glm::vec3& GetCenter() const { return m_center; }
	[[nodiscard]] const glm::vec3& GetHorizontalPixelDelta() const { return m_horizontalPixelDelta; }
	[[nodiscard]] const glm::vec3& GetVerticalPixelDelta() const { return m_verticalPixelDelta; }
	[[nodiscard]] const glm::vec3& GetPixel00Loc() const { return m_pixel00Loc; }
	[[nodiscard]] const bool& HasRotated() const { return m_hasRotated; }

	void Move(const glm::vec3& delta) { m_center += delta; m_lookAt += delta; m_pixel00Loc += delta; }

	void RecordInputs(GLFWwindow* window, float deltaTime);

private:
	float m_viewportWidth, m_viewportHeight;
	float m_focalLength = 1.f;
	float m_fov = 90.f;
	glm::vec3 m_center, m_lookAt, m_up, m_worldUp, m_u, m_v, m_w;
	glm::vec3 m_pixel00Loc, m_horizontalPixelDelta, m_verticalPixelDelta;
	glm::vec3 m_forward = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 m_right = glm::vec3(1.f, 0.f, 0.f);
	glm::vec2 m_previousMousePos = glm::vec2(0.f);
	glm::vec2 m_deltaMousePos = glm::vec2(0.f);
	bool m_firstMouseInput = true;
	bool m_hasRotated = false;
	float m_worldPitch = 0.f;
	float m_worldYaw = 0.f;

	void MoveInputs(GLFWwindow* window, float deltaTime);
	void LookInput(GLFWwindow* window, float deltaTime);
};