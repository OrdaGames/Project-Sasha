#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GL/glfw.h>

#include "game.h"

bool init();
void cleanup();

int main()
{

	if (init())
	{

		Game *game = new Game();

		game->mainLoop();

		delete game;
	}

	cleanup();

	return 0;
}


bool init()
{

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return false;
	}

	return true;
}

void cleanup()
{

}
