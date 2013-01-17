#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <lua5.2/lua.hpp>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <cstring>

namespace Util
{

	GLuint loadTGA(const char *filePath);

    class Keyboard
    {
        public:
            static void keyPressEvent(int key, int isPressed);
    };

    class Mouse
    {

        private:
            static int scrollWheelPosition;

        public:
            static void mouseMoved(int x, int y);
            static void mousePressed(int button, int isPressed);
            static void mouseWheelScrolled(int position);
    };

    class Gamepad
    {

        private:
            int ID;
            int numOfAxes;
            int numOfButtons;
            bool isSynced;
            unsigned char *buttons;

        public:

            enum BUTTONS{JUMP, ATTACK_1};

            Gamepad();
            ~Gamepad();

            void tick();

    };

	class Camera
	{

		private:
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::vec3 position;
			glm::vec3 targetPosition;

			float horizontalRotation;
			float verticalRotation;
			float lastHorizontalRotation;
			float lastVerticalRotation;

		public:
			Camera();
			~Camera();

            static const float CAMERA_MIN_X = -1.48352986f; //-85 degrees
            static const float CAMERA_MAX_X = 1.57079633f; //90 degrees

			void adjustPosition(glm::vec3 velocity);
			void setPosition(glm::vec3 position);
			void setTargetPosition(glm::vec3 position);

			void tick(double deltaTime);
			glm::mat4 getProjectionMatrix();
			glm::mat4 getViewMatrix();
			glm::mat4 calculateMVP(glm::mat4 ModelMatrix);
	};

	namespace Shader
	{
		GLuint loadShaders(const char *vertPath, const char *fragPath);
	}

	namespace Network
	{

	}

	class Config
	{

		protected:
			 static float MOUSE_SENSITIVITY;
			 static int WINDOW_WIDTH;
			 static int WINDOW_HEIGHT;
			 static int CURRENT_FPS;

		public:
			Config();
			~Config();

			 void setFPS(int FPS);

			 static int getCurrentFPS();
			 static float getMouseSensitivity();
			 static int getWindowWidth();
			 static int getWindowHeight();
	};

	namespace Event
	{

		class Event
		{

		};

	}

    namespace Font {

        static FT_Library ft;
        static FT_Face face;
        static GLuint shaderID;
        static GLuint samplerID;
        static GLuint colorID;
        static GLuint vertexBufferID;
        static GLuint coordID;

        bool init();

        void render(const char *text, Atlas *atlas, float x, float y, float sx, float sy);
    }

	namespace Lua
	{

		int print(lua_State *luaState);
		int error(lua_State *luaState);
		int getWindowWidth(lua_State *luaState);
		int getWindowHeight(lua_State *luaState);
		int getCurrentFPS(lua_State *luaState);
        int getMouseSensitivity(lua_State *LuaState);

		class LuaState
		{

			private:
				lua_State *luaState;


			public:
				LuaState();
				~LuaState();

				void tick();
				bool actionPerformed(Event::Event *e);
				int getMemoryUsage();
				int getMemoryUsageB();
		};
	}



}

#endif
