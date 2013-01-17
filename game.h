#ifndef GAME_H
#define GAME_H

#include "util.h"
#include "world.h"
#include "ui.h"
#include "entity.h"

class Game
{

	public:

		Game();
		~Game();

		static World::World *world;
        static Util::Config *config;
        static UI::UIHandler *uiHandler;
        static Entity::EntityPlayer *mainPlayer;

        static const char *WINDOW_TITLE;

		void mainLoop();

		void update(double deltaTime);
		void render();

        bool createWindow();

};

#endif
