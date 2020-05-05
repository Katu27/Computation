#include "Component.h"

#include <iostream>

#include <SFML/Graphics.hpp>
using namespace sf;

int Component::setBlock(std::string type, std::string rotation)
{
    this->type = type;
    this->rotation = rotation;
    this->isPowered = false;
    this->isPowered2 = false;
}

int Component::deleteBlock()
{
    type = "empty";
    rotation = "right";
    isPowered = false;
    isPowered2 = false;
}

int Component::rotateBlock()
{
    if(type != "switch" && type != "screen" && "wireJunction" && "wire4way")
    {
        if(type == "wire")
        {
            if(rotation == "right")
                rotation = "down";
            else if(rotation == "down")
                rotation = "right";
        }
        else
        {
            if(rotation == "right")
                rotation = "down";
            else if(rotation == "down")
                rotation = "left";
            else if(rotation == "left")
                rotation = "up";
            else if(rotation == "up")
                rotation = "right";
        }
    }
}
