#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

#include "util.h"

namespace Entity
{

    class Entity
    {

        protected:
            glm::vec3 position;
            float verticalRotation;
            float horizontalRotation;

        public:
            virtual void tick(double deltaTime) = 0;
            virtual void render() = 0;

    };

    class EntityPlayer : public Entity
    {

        private:
            Util::Camera *linkedCamera;
            static const float MAX_SPEED = 1.0f;

        public:

            EntityPlayer();

            void linkCamera(Util::Camera *camera);

            void tick(double deltaTime);
            void render();

            Util::Camera *getCamera();
    };

    class EntityMultiplayer : public EntityPlayer
    {

    };

}

#endif
