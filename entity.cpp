#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "entity.h"

Entity::EntityPlayer::EntityPlayer()
{
    position = glm::vec3(1,0,1);
    verticalRotation  = 0.0f;
	horizontalRotation = 0.0f;
}

void Entity::EntityPlayer::linkCamera(Util::Camera *camera)
{
    linkedCamera = camera;
}

void Entity::EntityPlayer::tick(double deltaTime)
{
    glm::vec3 direction(
            cos(verticalRotation) * sin(horizontalRotation),
            sin(verticalRotation),
            cos(verticalRotation) * cos(horizontalRotation)
        );

    glm::vec3 right(
            cos(verticalRotation - 3.14159f/2.0f),
            0,
            cos(verticalRotation - 3.14159f/2.0f)
        );

    if (glfwGetKey('W') == GLFW_PRESS)
    {
        position.x += MAX_SPEED * direction.x * deltaTime;
        position.z += MAX_SPEED * direction.z * deltaTime;
    }

    if (glfwGetKey('S') == GLFW_PRESS)
    {
        position.x -= MAX_SPEED * direction.x * deltaTime;
        position.z -= MAX_SPEED * direction.z * deltaTime;
    }

    if (glfwGetKey('D') == GLFW_PRESS)
    {
        verticalRotation += 0.51f * deltaTime;
    }

    if (glfwGetKey('A') == GLFW_PRESS)
    {
        verticalRotation -= 0.51f * deltaTime;
        //position.x += MAX_SPEED * right.x * deltaTime;
        //position.z += MAX_SPEED * right.z * deltaTime;
    }



    linkedCamera->setTargetPosition(position);
    linkedCamera->tick(deltaTime);
}

void Entity::EntityPlayer::render()
{

}

Util::Camera *Entity::EntityPlayer::getCamera()
{
    return linkedCamera;
}
