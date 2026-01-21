#include "Camera.h"

#include "glm/detail/func_geometric.inl"
#include "glm/detail/func_trigonometric.inl"
#include "spdlog/spdlog.h"

Camera::Camera(const uint16_t imageWidth, const uint16_t imageHeight, const float viewportHeight,
               const glm::vec3& center, const glm::vec3& lookAt, const glm::vec3& up,
	const float vFov) : m_viewportHeight(viewportHeight), m_fov(vFov), m_center(center), m_lookAt(lookAt), m_up(up), m_worldUp(up)
{
	m_forward = glm::normalize(m_lookAt - m_center);
	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_focalLength = static_cast<float>(glm::length((m_center - m_lookAt)));
	float theta = glm::radians(m_fov);
	float h = glm::tan(theta / 2.f);
	m_viewportHeight = 2.f * h * m_focalLength;
	m_viewportWidth = (static_cast<float>(imageWidth) / static_cast<float>(imageHeight)) * m_viewportHeight;

	m_w = glm::normalize(m_center - m_lookAt);
	m_u = glm::normalize(glm::cross(m_up, m_w));
	m_v = glm::cross(m_w, m_u);

	glm::vec3 horizontal = m_viewportWidth * m_u;
	glm::vec3 vertical = m_viewportHeight * m_v;
	glm::vec3 upperLeftCorner = m_center - (m_focalLength * m_w) - (horizontal / 2.f) + (vertical / 2.f);

	m_horizontalPixelDelta = horizontal / static_cast<float>(imageWidth);
	m_verticalPixelDelta = vertical / static_cast<float>(imageHeight);

	m_pixel00Loc = upperLeftCorner + 0.5f * (m_horizontalPixelDelta + m_verticalPixelDelta);
}

void Camera::RecordInputs(GLFWwindow* window, float deltaTime)
{
	MoveInputs(window, deltaTime);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_firstMouseInput = true;
		return;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	LookInput(window, deltaTime);
}

void Camera::MoveInputs(GLFWwindow* window, float deltaTime)
{
	glm::vec3 move = glm::vec3(0.f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		move += m_forward * 5.f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		move -= m_forward * 5.f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		move += m_right * 5.f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		move -= m_right * 5.f;

	Move(move * deltaTime);
}

void Camera::LookInput(GLFWwindow* window, float deltaTime)
{
	(void)deltaTime;
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	spdlog::info("Mouse Position: X: {}, Y: {}", xpos, ypos);
	if (m_firstMouseInput)
	{
		m_previousMousePos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
		m_firstMouseInput = false;
	}
	m_deltaMousePos.x = static_cast<float>(xpos) - m_previousMousePos.x;
	m_deltaMousePos.y = static_cast<float>(ypos) - m_previousMousePos.y; // reversed since y-coordinates go from bottom to top

	m_previousMousePos = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));

	m_hasRotated = m_deltaMousePos.x > 0.001f || m_deltaMousePos.y > 0.001f;

	if (!m_hasRotated)
		return;
	
	float pitch = m_deltaMousePos.y;
	float yaw = m_deltaMousePos.x;

	m_worldPitch += pitch;
	m_worldYaw += yaw;

	m_worldPitch = glm::clamp(m_worldPitch, -89.f, 89.f);
	glm::vec3 direction;
	direction.x = glm::cos(glm::radians(m_worldYaw)) * glm::cos(glm::radians(m_worldPitch));
	direction.y = glm::sin(glm::radians(m_worldPitch));
	direction.z = glm::sin(glm::radians(m_worldYaw)) * glm::cos(glm::radians(m_worldPitch));
	m_forward = glm::normalize(direction);
	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_forward));
	m_lookAt = m_center + m_forward;

	m_w = glm::normalize(m_center - m_lookAt);
	m_u = glm::normalize(glm::cross(m_up, m_w));
	m_v = glm::cross(m_w, m_u);
	glm::vec3 horizontal = m_viewportWidth * m_u;
	glm::vec3 vertical = m_viewportHeight * m_v;
	glm::vec3 upperLeftCorner = m_center - (m_focalLength * m_w) - (horizontal / 2.f) + (vertical / 2.f);
	m_pixel00Loc = upperLeftCorner + 0.5f * (m_horizontalPixelDelta + m_verticalPixelDelta);
}
