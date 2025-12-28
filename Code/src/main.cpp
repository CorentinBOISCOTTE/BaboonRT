#include <iostream>

#include "spdlog/spdlog.h"

int main()
{
#if defined(BABOON_DEBUG)
	spdlog::set_level(spdlog::level::debug);
	spdlog::info("Debug Build");
#else
	spdlog::info("Release Build");
#endif

	std::cout << "Hello, World!\n";
	return 0;
}
