#ifndef UI_H
#define UI_H

#include "util.h"

#include <vector>
#include <iostream>

namespace UI
{

    class Element
    {

        protected:
            int ID;
            int x, y;
            int width, height;
            bool hasFocus;
            GLuint bufferID;
            GLuint shaderID;
            GLuint halfResID;

        public:
            Element(int ID);
            Element(int ID, int x, int y, int width, int height);

            virtual void tick(double deltaTime) = 0;
            virtual void render() = 0;

            void setX(int x);
            void setY(int y);
            void setWidth(int width);
            void setHeight(int height);

            virtual bool handleKeyPress(int key, int isPressed) = 0;
            virtual bool handleMouseMove(int x, int y) = 0;
            virtual bool handleMouseClick(int x, int y, int buttonID) = 0;

            int getID();
            int getX();
            int getY();
            int getWidth();
            int getHeight();
    };

    class ElementTextBox : public Element
    {

        private:
            std::string text;

        public:
            ElementTextBox(int ID, int x, int y) : Element(ID)
            {
                this->x = x;
                this->y = y;

                height = 21;
                width = 450;

                const static GLint verts[] =
                {
                    x,   y, //Upper left
                    x,   y + height,
                    x + width, y,

                    x + width, y,
                    x + width, y + height,
                    x,   y + height
                };

                glGenBuffers(1, &bufferID);
                glBindBuffer(GL_ARRAY_BUFFER, bufferID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

                shaderID = Util::Shader::loadShaders("res/shaders/ui.vert", "res/shaders/ui.frag");

                halfResID = glGetUniformLocation(shaderID, "halfResolution");
            };

            void tick(double deltaTime);
            void render();
            void setText(const char *text);
            void setText(std::string text);

            std::string getText();
            const char * getTextC();
            bool handleKeyPress(int key, int isPressed);
            bool handleMouseMove(int x, int y);
            bool handleMouseClick(int x, int y, int buttonID);

    };


    class UIHandler
    {

        private:
            std::vector<UI::Element*> elements;
            double lastBackspace;

        public:

            UIHandler();
            ~UIHandler();

            void tick(double deltaTime);
            void render();

            void addElement(UI::Element *element);
            void removeElement(int ID);

            bool mousePressed(int x, int y, int button);
            bool keyPressed(int key, int isPressed);
            bool mouseMoved();
    };

}

#endif
