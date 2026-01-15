#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "ThreadPool.h"
#include "Sphere.h"

class Sphere;
class RayTracer;

class Application
{
public:
	Application() = default;
	~Application() = default;
	Application(uint32_t numThreads, const uint16_t width, const uint16_t height, const float viewportHeight, const glm::vec3& center, const float focalLength = 1.f, const float vFov = 90.f)
        : m_width(width), m_height(height), m_window(nullptr)
	{
        m_camera = new Camera(width, height, viewportHeight, center, focalLength, vFov);
		m_threadPool = new ThreadPool(numThreads);
    }

	void Initialize(const HittableList& world, int rayDepth);
	void Update();

private:
	uint16_t m_width = 1280;
	uint16_t m_height = 720;
	GLFWwindow* m_window;
    GLuint m_texture = 0;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLuint m_shader = 0;

    std::vector<Color> m_framebuffer;
    HittableList m_world;
	RayTracer* m_rayTracer = nullptr;
	Camera* m_camera = nullptr;
	ThreadPool* m_threadPool = nullptr;

    float m_deltaTime = 0.f;
    std::chrono::steady_clock::time_point m_lastTime;

    void Terminate() const;
    void CloseWindowInput() const;
    void CameraInput() const;
    void UpdateDeltaTime();

    void InitScreenQuad();
    void InitTexture();
    void DrawScreen() const;

    static GLuint CompileShader(GLenum type, const char* src);
    static GLuint CreateShaderProgram(const char* vs, const char* fs);
};

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
