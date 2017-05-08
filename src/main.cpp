#include "engine.hpp"
#include <cstdio>
#include <cstring>
#include <ctime>

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
	}
	catch (std::exception& e) {
		fprintf(stderr, "%s\n", e.what());
		system("pause");
		return -1;
	}
	catch (const char* str) {
		fprintf(stderr, "%s\n", str);
		system("pause");
		return -1;
	}
}