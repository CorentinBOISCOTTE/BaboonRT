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

	HittableList world;

	world.Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5));
	world.Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));

	Application app(std::thread::hardware_concurrency() - 1, 640, 480, 2.f, glm::vec3(0.f));

	app.Initialize(world, 10);

	app.Update();

	return 0;
}
