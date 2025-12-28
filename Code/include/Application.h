#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>

#include "Color.h"

class Sphere;
class RayTracer;

class Application
{
public:
	Application() = default;
	~Application() = default;
	Application(const uint16_t width, const uint16_t height)
	: m_width(width), m_height(height), m_window(nullptr) {}

	void Initialize(const std::vector<Sphere>& spheres);
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
	RayTracer* m_rayTracer = nullptr;

    void Terminate() const;
    void CloseWindowInput() const;

    void InitScreenQuad();
    void InitTexture();
    void DrawScreen() const;

    static GLuint CompileShader(GLenum type, const char* src);
    static GLuint CreateShaderProgram(const char* vs, const char* fs);
};

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
