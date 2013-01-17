#include <GL/glew.h>
#include <GL/glfw.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "util.h"
#include "game.h"

float Util::Config::MOUSE_SENSITIVITY = 0;
int Util::Config::CURRENT_FPS = 0;
int Util::Config::WINDOW_WIDTH = 1024;
int Util::Config::WINDOW_HEIGHT = 640;

int Util::Mouse::scrollWheelPosition = glfwGetMouseWheel();

void Util::Keyboard::keyPressEvent(int key, int isPressed)
{
    Game::uiHandler->keyPressed(key, isPressed);
}

void Util::Mouse::mouseMoved(int x, int y)
{
}

void Util::Mouse::mousePressed(int button, int isPressed)
{
    int mousePos[2];
    glfwGetMousePos(&mousePos[0], &mousePos[1]);

    mousePos[1] = 640 - mousePos[1];

    Game::uiHandler->mousePressed(mousePos[0], mousePos[1], 1);
}

void Util::Mouse::mouseWheelScrolled(int position)
{
        scrollWheelPosition = position;
}

Util::Gamepad::Gamepad()
{

    isSynced = false;

    for (int i=0; i <= GLFW_JOYSTICK_LAST; i++)
    {
        if (glfwGetJoystickParam(i, GLFW_PRESENT))
        {
            ID = i;
            isSynced = true;
            break;
        }
    }

    if (isSynced)
    {
        numOfButtons = glfwGetJoystickParam(ID, GLFW_BUTTONS);
        numOfAxes = glfwGetJoystickParam(ID, GLFW_AXES);

        buttons = new unsigned char[numOfButtons];

        std::cout << "Gamepad connected: [ID: " << ID << "] [Axes: " << numOfAxes << "] [Buttons: " << numOfButtons << "]" << std::endl;
    }

}

/*
 * @Method: Updates the current states of the buttons and axes of the gamepad
 */
void Util::Gamepad::tick()
{
    if (isSynced)
    {
        glfwGetJoystickButtons(ID, buttons, numOfButtons);
    }
}

Util::Camera::Camera()
{
	position = glm::vec3(4, 3, -3);

	ProjectionMatrix = glm::perspective(
			45.0f,
			4.0f / 3.0f,
			0.01f,
			100.0f
		);

	ViewMatrix = glm::lookAt(
			position,
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);

	verticalRotation = lastVerticalRotation = 0.0f;
	horizontalRotation = lastHorizontalRotation = 0.0f;

}

Util::Camera::~Camera()
{

}

void Util::Camera::adjustPosition(glm::vec3 velocity)
{
	position += velocity;
}

void Util::Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Util::Camera::setTargetPosition(glm::vec3 targetPosition)
{
    this->targetPosition = targetPosition;
}

glm::mat4 Util::Camera::getProjectionMatrix()
{
	return ProjectionMatrix;
}

glm::mat4 Util::Camera::getViewMatrix()
{
	return ViewMatrix;
}

glm::mat4 Util::Camera::calculateMVP(glm::mat4 ModelMatrix)
{
	return ProjectionMatrix * ViewMatrix * ModelMatrix;
}

void Util::Camera::tick(double deltaTime)
{

    ViewMatrix = glm::lookAt(
            position,
            targetPosition,
            glm::vec3(0, 1, 0)
        );

}

Util::Config::Config()
{
}

Util::Config::~Config()
{

}

void Util::Config::setFPS(int FPS)
{
	CURRENT_FPS = FPS;
}

int Util::Config::getWindowWidth()
{
	//Temp hardcoded
	return WINDOW_WIDTH;
}

int Util::Config::getWindowHeight()
{
	//temp hardcoded
	return WINDOW_HEIGHT;
}

int Util::Config::getCurrentFPS()
{
	return CURRENT_FPS;
}

float Util::Config::getMouseSensitivity()
{
    return MOUSE_SENSITIVITY;
}

GLuint Util::Shader::loadShaders(const char *vertPath, const char *fragPath)
{

	GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertSource;
	std::ifstream vertInputStream(vertPath, std::ios::in);
	if (vertInputStream.is_open())
	{
		std::string inLine = "";
		while(getline(vertInputStream, inLine))
			vertSource += "\n" + inLine;
	}

	std::string fragSource;
	std::ifstream fragInputStream(fragPath, std::ios::in);
	if (fragInputStream.is_open())
	{
		std::string inLine = "";
		while(getline(fragInputStream, inLine))
			fragSource += "\n" + inLine;
	}

	GLint result = false;
	int infoLogLength;

	const char *pVertSource = vertSource.c_str();
	glShaderSource(vertexID, 1, &pVertSource, NULL);
	glCompileShader(vertexID);

	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> vertErrorMessage(infoLogLength);
	glGetShaderInfoLog(vertexID, infoLogLength, NULL, &vertErrorMessage[0]);
	std::cout << &vertErrorMessage[0] << std::endl;

	const char *pFragSource = fragSource.c_str();
	glShaderSource(fragmentID, 1, &pFragSource, NULL);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> fragErrorMessage(infoLogLength);
	glGetShaderInfoLog(fragmentID, infoLogLength, NULL, &fragErrorMessage[0]);
	std::cout << &fragErrorMessage[0] << std::endl;

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));
	glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
	std::cout << programErrorMessage[0] << std::endl;

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	return programID;
}

Util::Lua::LuaState::LuaState()
{
	luaState = luaL_newstate();

	static const luaL_Reg luaLibs[] = {
		{"base", luaopen_base},
		{NULL, NULL}
	};

	const luaL_Reg *libraries = luaLibs;

	//Set which libraries the Lua VM is allowed to use. We prevent IO from being loaded to help prevent the creation of malicious scripts.
	for (; libraries->func != NULL; libraries++)
	{
		luaL_requiref(luaState, libraries->name, libraries->func, 1);
		lua_settop(luaState, 0);
	}

	static const luaL_Reg engineLibs[] = {
		{"print", Util::Lua::print},
		{"getWindowWidth", Util::Lua::getWindowWidth},
		{"getWindowHeight", Util::Lua::getWindowHeight},
		{"getCurrentFPS", Util::Lua::getCurrentFPS}
	};


	//Register every single method of the engine that lua can touch
	for (int i=0; i < 4; i++)
	{
		lua_register(luaState, engineLibs[i].name, engineLibs[i].func);
	}

	//Load and run the script
	int status = luaL_loadfile(luaState, "res/scripts/helloworld.lua");

	int result;

	if (status == LUA_OK)
	{
		result = lua_pcall(luaState, 0, LUA_MULTRET, 0);
	}

	else
	{
		std::cout << lua_tostring(luaState, -1);
		lua_pop(luaState, 1);
	}

}

Util::Lua::LuaState::~LuaState()
{
    lua_close(luaState);
}

void Util::Lua::LuaState::tick()
{

	lua_getglobal(luaState, "tick");
	if(lua_isfunction(luaState, -1))
	{
		lua_call(luaState, 0, 0);
	}
	else
	{
		lua_pop(luaState, 1);
	}

}

int Util::Lua::LuaState::getMemoryUsage()
{
	return lua_gc(luaState, LUA_GCCOUNT, 0);
}

int Util::Lua::LuaState::getMemoryUsageB()
{
	return lua_gc(luaState, LUA_GCCOUNTB, 0);
}

/*
 *	LUA SCRIPTING STARTS HERE
 *
 *	This is where every single method that is open to Lua will be wrapped
 *
 */


/*
 *	@Method: Prints from lua to the OpenGL console
 *	@Usage:	print("insert", "data", "here")
 *  @Return: Returns nothing, but outputs each argument on a new line
 */
int Util::Lua::print(lua_State *luaState)
{

	int argc = lua_gettop(luaState);

	for (int i=0; i < argc; i++)
	{

		//Game::font->renderString(lua_tostring(luaState, lua_gettop(luaState)), 0, 586, 14);

		lua_pop(luaState, 0);
	}

	return 0;
}

int Util::Lua::error(lua_State *luaState)
{
	int argc = lua_gettop(luaState);

	for (int i=0; i < argc; i++)
	{
		std::cout << "Error: " << lua_tostring(luaState, lua_gettop(luaState)) << std::endl;
		lua_pop(luaState, 0);
	}


	return 0;
}


/*
 * @Method: Gets the width of the OpenGL context
 * @Usage: height = getWindowWidth();
 * @Retun: Returns one integer, the width of the context
 */
int Util::Lua::getWindowWidth(lua_State *luaState)
{

	lua_pushnumber(luaState, Util::Config::getWindowWidth());

	return 1;
}

/*
 * @Method: Gets the height of the OpenGL context
 * @Usage: height = getWindowHeight();
 * @Retun: Returns one integer, the height of the context
 */
int Util::Lua::getWindowHeight(lua_State *luaState)
{

	lua_pushnumber(luaState, Util::Config::getWindowHeight());


	return 1;
}

int Util::Lua::getCurrentFPS(lua_State *luaState)
{

	lua_pushnumber(luaState, Util::Config::getCurrentFPS());

	return 1;
}

int Util::Lua::getMouseSensitivity(lua_State *luaState)
{
    lua_pushnumber(luaState, Util::Config::getMouseSensitivity());

    return 1;
}

GLuint Util::loadTGA(const char *filePath)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glfwLoadTexture2D(filePath, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

bool Util::Font::init()
{
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "Failed to load Freetype!" << std::endl;
        return false;
    }

    if (FT_New_Face(ft, "res/fonts/Ubuntu-R.ttf", 0 , &face))
    {
        std::cout << "Failed to load font file!" << std::endl;
        return false;
    }

    shaderID = Util::Shader::loadShaders("res/shaders/font.vert", "res/shaders/font.frag");

    samplerID = glGetUniformLocation(shaderID, "textureSampler");
    colorID = glGetUniformLocation(shaderID, "color");

    glGenBuffers(1, &vertexBufferID);

    font12px = new Atlas(face, 12);
    font24px = new Atlas(face, 24);
    font48px = new Atlas(face, 48);

    return true;
}

void Util::Font::render(const char *text, Atlas *a, float x, float y, float sx, float sy)
{
    glUseProgram(shaderID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const uint8_t *p;

	/* Use the texture containing the atlas */
	glBindTexture(GL_TEXTURE_2D, a->tex);
	glUniform1i(samplerID, 0);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	point coords[6 * strlen(text)];
	int c = 0;

	/* Loop through all characters */
	for (p = (const uint8_t *)text; *p; p++) {
		/* Calculate the vertex and texture coordinates */
		float x2 = x + a->c[*p].bl * sx;
		float y2 = -y - a->c[*p].bt * sy;
		float w = a->c[*p].bw * sx;
		float h = a->c[*p].bh * sy;

		/* Advance the cursor to the start of the next character */
		x += a->c[*p].ax * sx;
		y += a->c[*p].ay * sy;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;

		coords[c++] = (point) {
		x2, -y2, a->c[*p].tx, a->c[*p].ty};
		coords[c++] = (point) {
		x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty};
		coords[c++] = (point) {
		x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h};
		coords[c++] = (point) {
		x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty};
		coords[c++] = (point) {
		x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h};
		coords[c++] = (point) {
		x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h};
	}

	/* Draw all the character on the screen in one go */
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, c);

	glDisableVertexAttribArray(0);


}
