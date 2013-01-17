#ifndef WORLD_H
#define WORLD_H

#include <GL/glew.h>

#include "util.h"

namespace World
{

	class Voxel;

	typedef struct
	{
		Voxel *ptr[8];
	} Children;

	class Voxel
	{

		protected:
			Voxel *parent;
			int *data;
			Children *children;

		public:
			Voxel(Voxel *parent);
			~Voxel();

			Voxel * getParent();
			Voxel * getChild(int offset);
			Children * getChildren();
	};

    class Chunk
    {

        private:
            float x, y;

            void compileBuffers();

        public:
            Chunk(float x, float y);

            void render();

            static const int WIDTH = 16;
            static const int DEPTH = 16;
            static const int HEIGHT = 1;
    };

	class World
	{

		protected:
			GLuint cubeVertexBuffer;
			GLuint cubeColorBuffer;
			GLuint MatrixID;
			GLuint programID;

		public:
			World();

			void tick(double deltaTime);
			void render(Util::Camera *camera);
	};

	class WorldHander
	{

	};

}

#endif
