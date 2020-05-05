#ifndef LEVEL_H
#define LEVEL_H

#define __MaxN 102
#include "Component.h"

#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;

class Level
{
    public:
        int sizeX, sizeY;
        Sprite floor;
        RectangleShape hitbox;
        Component component[__MaxN][__MaxN];

        //Coordonatele tuturor block-urilor != wire-uri:
        std::vector< std::pair<int,int> >comp;
        int v[23104];
        int dj[4]={-1, 0, 1, 0};
        int di[4]={0, 1, 0, -1};
        int isVisited[__MaxN][__MaxN];
        int isLoop[__MaxN][__MaxN];
    public:
        Level(int sizeX, int sizeY);
        int update();
        void Fill(std::pair<int,int> nr, int x);
        int daRealFill(std::pair<int,int> nr, int x);
        void clearMap();
        void setTexture(RectangleShape* block, std::string type, std::string rotation, bool isPowered, bool isPowered2);
};

#endif
