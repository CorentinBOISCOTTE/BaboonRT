#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <vector>

class Application
{
public:
	Application() = default;
	~Application() = default;
	void Initialize(uint16_t width, uint16_t height);
	void Update();

private:
	uint16_t m_width;
	uint16_t m_height;
	GLFWwindow* m_window;
    GLuint m_texture = 0;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLuint m_shader = 0;

    std::vector<uint32_t> m_framebuffer;

    static void Terminate();
    void CloseWindowInput() const;

    void InitScreenQuad();
    void InitTexture();
    void DrawScreen() const;

    static GLuint CompileShader(GLenum type, const char* src);
    static GLuint CreateShaderProgram(const char* vs, const char* fs);
};

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
