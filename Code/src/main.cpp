#include <iostream>

#include "Application.h"
#include "spdlog/spdlog.h"

int main()
{
#if defined(BABOON_DEBUG)
	spdlog::set_level(spdlog::level::debug);
	spdlog::info("Debug Build");
#else
	spdlog::info("Release Build");
#endif

	Application app;

	app.Initialize(1280, 720);

	app.Update();

	return 0;
}
