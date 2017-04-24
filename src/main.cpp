// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "engine.hpp"
#include <cstdio>
#include <cstring>
#include <ctime>
#include<lua.hpp>
int main(int argc, char** argv) {
	srand(time(NULL));
	bool vsync = true;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--vsync"))
			vsync = true;
		else if (!strcmp(argv[i], "--disable-vsync"))
			vsync = false;
	}

	try {
		return Engine::getInstance().run(vsync);
	} catch (std::exception& e) {
		fprintf(stderr, "%s\n", e.what());
		getchar();
		return -1;
	} catch (const char* str) {
		fprintf(stderr, "%s\n", str);
		getchar();
		return -1;
	}
}
