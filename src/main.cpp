#include "engine.hpp"
#include <cstdio>
#include <cstring>
#include <ctime>

#ifdef _WIN32
#define PAUSE() do{ system("pause"); } while(false)
#else
#define PAUSE() do{ __asm__("int3"); } while(false)
#endif

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
	} catch (const std::exception& e) {
		fprintf(stderr, "%s\n", e.what());
		PAUSE();
		return -1;
	} catch (const char* str) {
		fprintf(stderr, "%s\n", str);
		PAUSE();
		return -1;
	}
}
