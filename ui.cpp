#include <GL/glew.h>
#include <GL/glfw.h>

#include <string>

#include "ui.h"
#include "game.h"
#include "util.h"

UI::Element::Element(int ID)
{
    this->ID = ID;

    x = y = 180;
    width = height = 300;

    hasFocus = false;

}

void UI::Element::setX(int x)
{
    this->x = x;
}

void UI::Element::setY(int y)
{
    this->y = y;
}

void UI::Element::setWidth(int width)
{
    this->width = width;
}

void UI::Element::setHeight(int height)
{
    this->height = height;
}

int UI::Element::getID()
{
    return ID;
}

int UI::Element::getX()
{
    return x;
}

int UI::Element::getY()
{
    return y;
}

int UI::Element::getWidth()
{
    return width;
}

int UI::Element::getHeight()
{
    return height;
}

/******** END OF THE BASE ENTITY CLASS ********/



/*
 * @Begin: ElementTextBox
 * @Purpose: This element is used to get text input.
 */
bool UI::ElementTextBox::handleKeyPress(int key, int isPressed)
{

    if (hasFocus)
    {
        if (key == 0x08)//Backspace
            text = text.substr(0, text.length() - 1);

        else
            text += key;
    }

    return false;
}

bool UI::ElementTextBox::handleMouseMove(int x, int y)
{

    //We wont do anything if the mouse is hovered over this element type

    return false;
}

bool UI::ElementTextBox::handleMouseClick(int x, int y, int buttonID)
{

    if ((x >= this->x && x <= (this->x + width)) && (y >= this->y && y <= (this->y + height)))
    {
        hasFocus = true;
        return true;
    }
    hasFocus = false;
    return false;
}

void UI::ElementTextBox::setText(const char *text)
{
    this->text = std::string(text);
}

void UI::ElementTextBox::setText(std::string text)
{
    this->text = text;
}

void UI::ElementTextBox::render()
{
    glUseProgram(shaderID);

    float halfRes[] = {512, 320};
    glUniform2fv(halfResID, 1, halfRes);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glVertexAttribPointer(
            0,
            2,
            GL_INT,
            GL_FALSE,
            0,
            (void*)0
        );

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
}

void UI::ElementTextBox::tick(double deltaTime)
{
    //Eventually we will do a blinking cursor
}

std::string UI::ElementTextBox::getText()
{
    return text;
}

//This returns the C-string version of the text
const char *UI::ElementTextBox::getTextC()
{
    return text.c_str();
}

/*
 *
 *
 */

UI::UIHandler::UIHandler()
{
    lastBackspace = 0;
}

UI::UIHandler::~UIHandler()
{
}

void UI::UIHandler::addElement(UI::Element *element)
{
    elements.push_back(element);
}

void UI::UIHandler::removeElement(int ID)
{
    if (!elements.empty())
    {
        for (unsigned int i=0; i < elements.size(); i++)
        {
            if (elements[i]->getID() == ID)
            {
                //remove this element
                break;
            }
        }
    }
}

bool UI::UIHandler::keyPressed(int key, int isPressed)
{

    if (!elements.empty())
    {
        for (unsigned int i=0; i < elements.size(); i++)
        {
            elements[i]->handleKeyPress(key, isPressed);
        }
    }

    return true;
}

bool UI::UIHandler::mousePressed(int x, int y, int buttonID)
{
    if (!elements.empty())
    {
        for (unsigned int i=0; i < elements.size(); i++)
        {
            elements[i]->handleMouseClick(x, y, buttonID);
        }

        return true;
    }

    return false;
}

void UI::UIHandler::tick(double deltaTime)
{
    if (!elements.empty())
    {

        if (glfwGetTime() - lastBackspace >= 0.09)
        {
            if (glfwGetKey(GLFW_KEY_BACKSPACE) == GLFW_PRESS)
            {
                keyPressed(0x08, 1); //backspace
                lastBackspace = glfwGetTime();
            }
        }


        for (unsigned int i=0; i < elements.size(); i++)
        {
            elements[i]->tick(deltaTime);
        }
    }
}

void UI::UIHandler::render()
{
    if (!elements.empty())
    {
        for (unsigned int i=0; i < elements.size(); i++)
        {
            elements[i]->render();
        }
    }
}
