#include <GL/glew.h>
#include <GL/glfw.h>

#include <iostream>

#include "game.h"
#include "ui.h"

World::World *Game::world = NULL;
Util::Config *Game::config = NULL;
UI::UIHandler *Game::uiHandler = NULL;
Entity::EntityPlayer *Game::mainPlayer = NULL;

const char *Game::WINDOW_TITLE = "Project Sasha";

Game::Game()
{

    createWindow();

    Util::Font::init();

	world = new World::World();
    config = new Util::Config();
    uiHandler = new UI::UIHandler();
    uiHandler->addElement(new UI::ElementTextBox(0, 0, 10));

    mainPlayer = new Entity::EntityPlayer();
    mainPlayer->linkCamera(new Util::Camera());

	GLfloat black[4] = { 1, 1, 1, 1 };
	GLfloat red[4] = { 1, 0, 0, 1 };
	GLfloat transparent_green[4] = { 0, 1, 0, 0.5 };

	glUniform4fv(Util::Font::colorID, 1, black);
}

Game::~Game()
{
	delete world;
	delete uiHandler;
	delete config;
	delete mainPlayer;
}

void Game::mainLoop()
{
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	double lastUpdate = 0;
    //double lastCommand;
    int frame = 0;
    int frameRate = 0;
    double lastFrameCount;

    Util::Lua::LuaState *fpsScript = new Util::Lua::LuaState();

	while (true)
	{

		double deltaTime = (glfwGetTime() * 1000) - (lastUpdate * 1000); //On my Intel i7, this thread was running so fast it caused a problem because of double inaccuracy.

		update(deltaTime);

		render();

        fpsScript->tick();

		lastUpdate = glfwGetTime();

        if (glfwGetKey('L') == GLFW_PRESS)
        {
            delete fpsScript;
            fpsScript = new Util::Lua::LuaState();
        }

		//glfwSetMousePos(1024/2, 640/2);

        if(glfwGetTime() * 1000 - lastFrameCount * 1000 >= 1000)
        {
            frameRate = frame;
            frame = 0;
            config->setFPS(frameRate);
            lastFrameCount = glfwGetTime();
        }
        else
            frame++;

		if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS || !glfwGetWindowParam(GLFW_OPENED))
			break;

		glfwSwapBuffers();

	}
}

bool Game::createWindow()
{

    glfwCloseWindow();

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	//glfwOpenWindowHint(GLFW_REFRESH_RATE, 60);

    if (!glfwOpenWindow(config->getWindowWidth(), config->getWindowHeight(), 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	{
		std::cout << "Failed top open GLFW window. Intel and older GPU's do not support OpenGL 3.3" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwSetWindowTitle(WINDOW_TITLE);

	glfwEnable(GLFW_STICKY_KEYS);

    glfwSetCharCallback(Util::Keyboard::keyPressEvent);
    glfwSetMouseWheelCallback(Util::Mouse::mouseWheelScrolled);
    glfwSetMouseButtonCallback(Util::Mouse::mousePressed);
    glfwSetMousePosCallback(Util::Mouse::mouseMoved);

	glfwSwapInterval(0);

    glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    return true;

}

void Game::update(double deltaTime)
{
    uiHandler->tick(deltaTime);
    mainPlayer->tick(deltaTime);
}

void Game::render()
{
    world->render(mainPlayer->getCamera());
    uiHandler->render();

    Util::Font::render("The Transparent Green Fox Jumps Over The Lazy Dog", Util::Font::font12px, -1 + 8 * (2.0f / 1024), 1 - 380 * (2.0 / 640), (2.0f / 1024), (2.0 / 640));

}
