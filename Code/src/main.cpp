#include <iostream>

#include "Application.h"
#include "Sphere.h"
#include "glm/vec3.hpp"
#include "spdlog/spdlog.h"

int main()
{
#if defined(BABOON_DEBUG)
	spdlog::set_level(spdlog::level::debug);
	spdlog::info("Debug Build");
#else
	spdlog::info("Release Build");
#endif

	Sphere sphere1(1.0f, glm::vec3(0.0f, 0.0f, -5.0f));

	const std::vector<Sphere> spheres = { sphere1 };

	Application app(640, 480);

	app.Initialize(spheres);

	app.Update();

	return 0;
}
