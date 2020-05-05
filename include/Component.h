#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <SFML/Graphics.hpp>
using namespace sf;

class Component
{
    public:
        std::string type;
        std::string rotation;
        bool isPowered; // horizontal
        bool isPowered2; // vertical

    public:
        int setBlock(std::string type, std::string rotation = "right");
        int deleteBlock();
        int rotateBlock();
};

#endif
