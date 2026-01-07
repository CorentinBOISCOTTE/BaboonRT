#include <iostream>

#include "Application.h"
#include "Material.h"
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

	//world.Add(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5));
	//world.Add(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));

	auto material_ground = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
	auto material_center = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
	auto material_left = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), .3f);
	auto material_right = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.f);

	world.Add(std::make_shared<Sphere>(glm::vec3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.Add(std::make_shared<Sphere>(glm::vec3(0.0, 0.0, -1.2), 0.5, material_center));
	world.Add(std::make_shared<Sphere>(glm::vec3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.Add(std::make_shared<Sphere>(glm::vec3(1.0, 0.0, -1.0), 0.5, material_right));

	Application app(std::thread::hardware_concurrency() - 1, 640, 480, 2.f, glm::vec3(0.f));

	app.Initialize(world, 500, 100);

	app.Update();

	return 0;
}
