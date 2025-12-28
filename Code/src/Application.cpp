#include "Application.h"

#include "glad/glad.h"

#include <iostream>
#include <print>
#include <fstream>

#include <cstdlib>
#include <ctime>

#include "spdlog/spdlog.h"

void Application::Initialize(uint16_t width, uint16_t height)
{
	m_width = width;
	m_height = height;

	if (!glfwInit())
		return;

	m_window = glfwCreateWindow(m_width, m_height, "BaboonRT", nullptr, nullptr);

	if (!m_window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		return;
	}

	m_framebuffer.resize(m_width * m_height, 0xFF000000);

	InitTexture();
	InitScreenQuad();

	std::string vertLine, vertTxt;
	std::ifstream vertIn("Assets/Shaders/BaboonRT.vert");
	while (std::getline(vertIn, vertLine))
	{
		vertTxt += vertLine + "\n";
	}
	const char* vertexSrc = vertTxt.c_str();

	std::string fragLine, fragTxt;
	std::ifstream fragIn("Assets/Shaders/BaboonRT.frag");
	while (std::getline(fragIn, fragLine))
	{
		fragTxt += fragLine + "\n";
	}
	const char* fragmentSrc = fragTxt.c_str();

	m_shader = CreateShaderProgram(vertexSrc, fragmentSrc);

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	spdlog::debug("GL_VENDOR: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	spdlog::debug("GL_VERSION: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	spdlog::debug("GL_RENDERER: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}

void Application::Update()
{
	while (!glfwWindowShouldClose(m_window))
	{
		CloseWindowInput();

		// Render to framebuffer
		for (uint32_t y = 0; y < m_height; y++)
		{
			for (uint32_t x = 0; x < m_width; x++)
			{
				//uint8_t r = uint8_t(float(x) / m_width * 255);
				//uint8_t g = uint8_t(float(y) / m_height * 255);
				//m_framebuffer[y * m_width + x] =
				//	0xFF000000 | (g << 8) | r;

				uint8_t r = std::rand() % 256; // random 0-255
				uint8_t g = std::rand() % 256;
				uint8_t b = std::rand() % 256;

				m_framebuffer[y * m_width + x] =
					0xFF000000 | (b << 16) | (g << 8) | r;
			}
		}

		// Upload pixels
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0, 0,
			m_width, m_height,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			m_framebuffer.data()
		);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		GLint loc = glGetUniformLocation(m_shader, "uScreen");
		glUniform1i(loc, 0);

		DrawScreen();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void Application::Terminate()
{
	glfwTerminate();
}

void Application::CloseWindowInput() const
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

}

void Application::InitScreenQuad()
{
	const float quad[] = {
		-1, -1,     0, 0,
		 1, -1,     1, 0,
		 1,  1,     1, 1,
		-1,  1,     0, 1
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
		reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Application::InitTexture()
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		m_width,
		m_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		m_framebuffer.data()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Application::DrawScreen() const
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLuint Application::CompileShader(GLenum type, const char* src)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		spdlog::error("Shader compile error:\n{}", log);
	}

	return shader;
}

GLuint Application::CreateShaderProgram(const char* vs, const char* fs)
{
	GLuint vert = CompileShader(GL_VERTEX_SHADER, vs);
	GLuint frag = CompileShader(GL_FRAGMENT_SHADER, fs);

	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetProgramInfoLog(program, 512, nullptr, log);
		spdlog::error("Program link error:\n{}", log);
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	return program;
}

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
	(void)window;
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}
