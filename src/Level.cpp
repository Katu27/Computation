#include "Level.h"

#include <thread>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <windows.h>
#define Nexti di[i] + nr.first
#define Nextj dj[i] + nr.second
//std::thread updateThread(level.update);

using namespace sf;

extern RenderWindow window;
extern int windowSizeX;
extern int windowSizeY;

extern Texture floorTexture;
extern Texture emptyTexture;

extern Texture switchOnTexture;
extern Texture switchOffTexture;

extern Texture screenOnTexture;
extern Texture screenOffTexture;

extern Texture wireOnrightTexture;
extern Texture wireOffrightTexture;
extern Texture wireOndownTexture;
extern Texture wireOffdownTexture;

extern Texture wireJunctionAlloff;
extern Texture wireJunctionAllpowered;
extern Texture wireJunctionHorizontalpowered;
extern Texture wireJunctionVerticalpowered;

extern Texture wireCornerOffrightTexture;
extern Texture wireCornerOffdownTexture;
extern Texture wireCornerOffleftTexture;
extern Texture wireCornerOffupTexture;
extern Texture wireCornerOnrightTexture;
extern Texture wireCornerOndownTexture;
extern Texture wireCornerOnleftTexture;
extern Texture wireCornerOnupTexture;

extern Texture wire3wayOffrightTexture;
extern Texture wire3wayOffdownTexture;
extern Texture wire3wayOffleftTexture;
extern Texture wire3wayOffupTexture;
extern Texture wire3wayOnrightTexture;
extern Texture wire3wayOndownTexture;
extern Texture wire3wayOnleftTexture;
extern Texture wire3wayOnupTexture;

extern Texture wire4wayOffrightTexture;
extern Texture wire4wayOnrightTexture;

extern Texture NOTrightTexture;
extern Texture NOTdownTexture;
extern Texture NOTleftTexture;
extern Texture NOTupTexture;

extern Texture ORrightTexture;
extern Texture ORdownTexture;
extern Texture ORleftTexture;
extern Texture ORupTexture;

extern Texture ANDrightTexture;
extern Texture ANDdownTexture;
extern Texture ANDleftTexture;
extern Texture ANDupTexture;

extern Texture XORrightTexture;
extern Texture XORdownTexture;
extern Texture XORleftTexture;
extern Texture XORupTexture;

extern Texture XANDrightTexture;
extern Texture XANDdownTexture;
extern Texture XANDleftTexture;
extern Texture XANDupTexture;

extern Texture NORrightTexture;
extern Texture NORdownTexture;
extern Texture NORleftTexture;
extern Texture NORupTexture;

extern Texture NANDrightTexture;
extern Texture NANDdownTexture;
extern Texture NANDleftTexture;
extern Texture NANDupTexture;

extern bool Fullscreen;

void Level::setTexture(RectangleShape* block, std::string type, std::string rotation, bool isPowered, bool isPowered2)
{
	if(type == "empty")
	{
		(*block).setTexture(&emptyTexture);
	}


	else if(type == "switch")
		if(isPowered)
			(*block).setTexture(&switchOnTexture);
		else
			(*block).setTexture(&switchOffTexture);


	else if(type == "screen")
		if(isPowered)
			(*block).setTexture(&screenOnTexture);
		else
			(*block).setTexture(&screenOffTexture);


	else if(type == "wire")
	{
		if(rotation == "right")
			if(isPowered)
				(*block).setTexture(&wireOnrightTexture);
			else
				(*block).setTexture(&wireOffrightTexture);
		else if(rotation == "down")
			if(isPowered)
				(*block).setTexture(&wireOndownTexture);
			else
				(*block).setTexture(&wireOffdownTexture);
	}


	else if(type == "wireCorner")
	{
		if(rotation == "right")
			if(isPowered)
				(*block).setTexture(&wireCornerOnrightTexture);
			else
				(*block).setTexture(&wireCornerOffrightTexture);
		else if(rotation == "down")
			if(isPowered)
				(*block).setTexture(&wireCornerOndownTexture);
			else
				(*block).setTexture(&wireCornerOffdownTexture);
		else if(rotation == "left")
			if(isPowered)
				(*block).setTexture(&wireCornerOnleftTexture);
			else
				(*block).setTexture(&wireCornerOffleftTexture);
		else if(rotation == "up")
			if(isPowered)
				(*block).setTexture(&wireCornerOnupTexture);
			else
				(*block).setTexture(&wireCornerOffupTexture);
	}

	else if(type == "wire3way")
	{
		if(rotation == "right")
			if(isPowered)
				(*block).setTexture(&wire3wayOnrightTexture);
			else
				(*block).setTexture(&wire3wayOffrightTexture);
		else if(rotation == "down")
			if(isPowered)
				(*block).setTexture(&wire3wayOndownTexture);
			else
				(*block).setTexture(&wire3wayOffdownTexture);
		else if(rotation == "left")
			if(isPowered)
				(*block).setTexture(&wire3wayOnleftTexture);
			else
				(*block).setTexture(&wire3wayOffleftTexture);
		else if(rotation == "up")
			if(isPowered)
				(*block).setTexture(&wire3wayOnupTexture);
			else
				(*block).setTexture(&wire3wayOffupTexture);
	}
	else if(type == "wire4way")
	{
		if(isPowered)
			(*block).setTexture(&wire4wayOnrightTexture);
		else
			(*block).setTexture(&wire4wayOffrightTexture);
	}
	else if(type == "NOTgate")
	{
		if(rotation == "right")
			(*block).setTexture(&NOTrightTexture);
		else if(rotation == "down")
			(*block).setTexture(&NOTdownTexture);
		else if(rotation == "left")
			(*block).setTexture(&NOTleftTexture);
		else if(rotation == "up")
			(*block).setTexture(&NOTupTexture);
	}

	else if(type == "ORgate")
	{
		if(rotation == "right")
			(*block).setTexture(&ORrightTexture);
		else if(rotation == "down")
			(*block).setTexture(&ORdownTexture);
		else if(rotation == "left")
			(*block).setTexture(&ORleftTexture);
		else if(rotation == "up")
			(*block).setTexture(&ORupTexture);
	}

	else if(type == "ANDgate")
	{
		if(rotation == "right")
			(*block).setTexture(&ANDrightTexture);
		else if(rotation == "down")
			(*block).setTexture(&ANDdownTexture);
		else if(rotation == "left")
			(*block).setTexture(&ANDleftTexture);
		else if(rotation == "up")
			(*block).setTexture(&ANDupTexture);
	}

	else if(type == "XORgate")
	{
		if(rotation == "right")
			(*block).setTexture(&XORrightTexture);
		else if(rotation == "down")
			(*block).setTexture(&XORdownTexture);
		else if(rotation == "left")
			(*block).setTexture(&XORleftTexture);
		else if(rotation == "up")
			(*block).setTexture(&XORupTexture);
	}

	else if(type == "XANDgate")
	{
		if(rotation == "right")
			(*block).setTexture(&XANDrightTexture);
		else if(rotation == "down")
			(*block).setTexture(&XANDdownTexture);
		else if(rotation == "left")
			(*block).setTexture(&XANDleftTexture);
		else if(rotation == "up")
			(*block).setTexture(&XANDupTexture);
	}

	else if(type == "NORgate")
	{
		if(rotation == "right")
			(*block).setTexture(&NORrightTexture);
		else if(rotation == "down")
			(*block).setTexture(&NORdownTexture);
		else if(rotation == "left")
			(*block).setTexture(&NORleftTexture);
		else if(rotation == "up")
			(*block).setTexture(&NORupTexture);
	}

	else if(type == "NANDgate")
	{
		if(rotation == "right")
			(*block).setTexture(&NANDrightTexture);
		else if(rotation == "down")
			(*block).setTexture(&NANDdownTexture);
		else if(rotation == "left")
			(*block).setTexture(&NANDleftTexture);
		else if(rotation == "up")
			(*block).setTexture(&NANDupTexture);
	}

	else if(type == "wireJunction")
	{
		if(isPowered && isPowered2)
			(*block).setTexture(&wireJunctionAllpowered);
		else if(!isPowered && !isPowered2)
			(*block).setTexture(&wireJunctionAlloff);
		else if(isPowered && !isPowered2)
			(*block).setTexture(&wireJunctionHorizontalpowered);
		else if(!isPowered && isPowered2)
			(*block).setTexture(&wireJunctionVerticalpowered);
	}
}

Level::Level(int sizeX, int sizeY)
{


	//Floor setup:
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	floor.setTexture(floorTexture);



	//RectangleShape setup:
	hitbox.setSize(Vector2f(64.0, 64.0));

	for(int x = 1; x < sizeX; x++)
	{

		for(int y = 1; y < sizeY; y++)
		{

			//Component setup:
			component[x][y].type = "empty";
			component[x][y].rotation = "right";
			component[x][y].isPowered = false;
			component[x][y].isPowered2 = false;
			isVisited[x][y] = 0;
		}
	}
}

int Level::daRealFill(std::pair<int, int> nr, int x)
{
	isVisited[nr.first][nr.second]++;
	if(isVisited[nr.first][nr.second] > 1 && component[nr.first][nr.second].type != "wireJunction")
		return 0;
	int sum = 0;
	int corecte = 0;
	bool ok2 = false;

	if(component[nr.first][nr.second].type == "screen")
		return 1;
	if(component[nr.first][nr.second].type == "empty" || nr.first > sizeX || nr.second > sizeY || nr.first < 0 || nr.second < 0)
		return 0;

	//Current is Switch-Real:
	if(component[nr.first][nr.second].type == "switch")
	{
		for(int i = 0; i <= 3; ++i)
		{
			if(component[Nexti][Nextj].type != "empty")
			{
				ok2 = false;

				//Next is wire:
				if(component[Nexti][Nextj].type == "wire")
				{
					ok2 = component[Nexti][Nextj].rotation == "right" && (i == 1 || i == 3) ? true : ok2;
					ok2 = component[Nexti][Nextj].rotation == "down" && (i == 0 || i == 2) ? true : ok2;
				}

				//Next is NOTgate:
				if(component[Nexti][Nextj].type == "NOTgate")
				{
					ok2 = component[Nexti][Nextj].rotation == "up" && i == 0 ? true : ok2;
					ok2 = component[Nexti][Nextj].rotation == "right" && i == 1 ? true : ok2;
					ok2 = component[Nexti][Nextj].rotation == "down" && i == 2 ? true : ok2;
					ok2 = component[Nexti][Nextj].rotation == "left" && i == 3 ? true : ok2;
				}

				//Next is ORgate:
				if(component[Nexti][Nextj].type == "ORgate")
				{
					std::string var = component[Nexti][Nextj].rotation;
					bool isVertical = false;
					isVertical = var == "up" || var == "down" ? true : false;
					ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
					ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				}

				//Next is ANDgate:
				if(component[Nexti][Nextj].type == "ANDgate")
				{
					std::string var = component[Nexti][Nextj].rotation;
					bool isVertical = false;
					isVertical = var == "up" || var == "down" ? true : false;
					ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
					ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				}

				//Next is XORgate:
				if(component[Nexti][Nextj].type == "XORgate")
				{
					std::string var = component[Nexti][Nextj].rotation;
					bool isVertical = false;
					isVertical = var == "up" || var == "down" ? true : false;
					ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
					ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				}

				//Next is XANDgate:
				if(component[Nexti][Nextj].type == "XANDgate")
				{
					std::string var = component[Nexti][Nextj].rotation;
					bool isVertical = false;
					isVertical = var == "up" || var == "down" ? true : false;
					ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
					ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				}

				//Next is NORgate:
				if(component[Nexti][Nextj].type == "NORgate")
				{
					std::string var = component[Nexti][Nextj].rotation;
					bool isVertical = false;
					isVertical = var == "up" || var == "down" ? true : false;
					ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
					ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				}

				//Next is NANDgate:
				if(component[Nexti][Nextj].type == "NANDgate")
				{
					std::string var = component[Nexti][Nextj].rotation;
					bool isVertical = false;
					isVertical = var == "up" || var == "down" ? true : false;
					ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
					ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				}

				//Next is wireCorner:
				if(component[Nexti][Nextj].type == "wireCorner")
				{
					ok2 = i == 0 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left") ? true : ok2;
					ok2 = i == 1 && (component[Nexti][Nextj].rotation == "left" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
					ok2 = i == 2 && (component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
					ok2 = i == 3 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "right") ? true : ok2;
				}

				//Next is wire3way:
				if(component[Nexti][Nextj].type == "wire3way")
				{
					std::string var = component[Nexti][Nextj].rotation;
					ok2 = i == 0 && var != "up" ? true : ok2;
					ok2 = i == 1 && var != "right" ? true : ok2;
					ok2 = i == 2 && var != "down" ? true : ok2;
					ok2 = i == 3 && var != "left" ? true : ok2;
				}

				//Next is wire4way:
				ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

				//Next is WireJunction:
				ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

				if(ok2 == true)
					sum += daRealFill({Nexti, Nextj}, i) + 1, corecte++;
			}
		}
		return sum - corecte + 1;
	}
	ok2 = false;

	//Current is wire-Real:
	if(component[nr.first][nr.second].type == "wire")
	{
		int i = -1;
		i = component[nr.first][nr.second].rotation == "right" && (x == 1 || x == 3) ? x : i;
		i = component[nr.first][nr.second].rotation == "down" && (x == 0 || x == 2) ? x : i;
		if(component[Nexti][Nextj].type != "empty" && component[Nexti][Nextj].type != "switch")
		{
			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				ok2 = component[Nexti][Nextj].rotation == component[nr.first][nr.second].rotation ? true : ok2;
			}

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				ok2 = component[nr.first][nr.second].rotation == "right" && (component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "left") ? true : ok2;
				ok2 = component[nr.first][nr.second].rotation == "down" && (component[Nexti][Nextj].rotation == "up" || component[Nexti][Nextj].rotation == "down") ? true : ok2;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XANDgate:
			if(component[Nexti][Nextj].type == "XANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NORgate:
			if(component[Nexti][Nextj].type == "NORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NANDgate:
			if(component[Nexti][Nextj].type == "NANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				ok2 = i == 0 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left") ? true : ok2;
				ok2 = i == 1 && (component[Nexti][Nextj].rotation == "left" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 2 && (component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 3 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "right") ? true : ok2;
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
			}

			//Next is wire4way:
			ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

			//Next is wireJunction:
			ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

			if(ok2 == true)
				return daRealFill({Nexti, Nextj}, i) + 1;
		}
		return 1;
	}

	ok2 = false;

	//Current is NOTgate-Real:
	if(component[nr.first][nr.second].type == "NOTgate")
	{
		int i = -1;
		i = component[nr.first][nr.second].rotation == "up" ? 0 : i;
		i = component[nr.first][nr.second].rotation == "right" ? 1 : i;
		i = component[nr.first][nr.second].rotation == "down" ? 2 : i;
		i = component[nr.first][nr.second].rotation == "left" ? 3 : i;
		if(component[Nexti][Nextj].type != "empty"  && component[Nexti][Nextj].type != "switch")
		{
			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				ok2 = (i == 0 || i == 2) && component[Nexti][Nextj].rotation == "down" ? true : ok2;
				ok2 = (i == 1 || i == 3) && component[Nexti][Nextj].rotation == "right" ? true : ok2;
			}

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				ok2 = (component[Nexti][Nextj].rotation == component[nr.first][nr.second].rotation) ? true : false;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				ok2 = i == 0 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left") ? true : ok2;
				ok2 = i == 1 && (component[Nexti][Nextj].rotation == "left" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 2 && (component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 3 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "right") ? true : ok2;
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
			}

			//Next is wire4way:
			ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

			//Next is wireJunction:
			ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

			if(ok2 == true)
				return daRealFill({Nexti, Nextj}, i) + 1;
		}
		return 1;
	}

	//Current is wireCorner-Real:
	if(component[nr.first][nr.second].type == "wireCorner")
	{
		int i;
		if(x == 0)
			i = (component[nr.first][nr.second].rotation == "down") ? 1 : 3;
		if(x == 1)
			i = (component[nr.first][nr.second].rotation == "left") ? 2 : 0;
		if(x == 2)
			i = (component[nr.first][nr.second].rotation == "up") ? 3 : 1;
		if(x == 3)
			i = (component[nr.first][nr.second].rotation == "right") ? 0 : 2;
		if(component[Nexti][Nextj].type != "empty"  && component[Nexti][Nextj].type != "switch")
		{

			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				ok2 = (i == 0 || i == 2) && component[Nexti][Nextj].rotation == "down" ? true : ok2;
				ok2 = (i == 1 || i == 3) && component[Nexti][Nextj].rotation == "right" ? true : ok2;
			}

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XANDgate:
			if(component[Nexti][Nextj].type == "XANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NORgate:
			if(component[Nexti][Nextj].type == "NORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NANDgate:
			if(component[Nexti][Nextj].type == "NANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				ok2 = i == 0 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left") ? true : ok2;
				ok2 = i == 1 && (component[Nexti][Nextj].rotation == "left" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 2 && (component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 3 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "right") ? true : ok2;
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
			}

			//Next is wire4way:
			ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

			//Next is wireJunction:
			ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

			if(ok2 == true)
				return daRealFill({Nexti, Nextj}, i) + 1;
		}
		return 1;
	}

	//Current is wire3way-Real:
	if(component[nr.first][nr.second].type == "wire3way")
	{
		int sum = 0;
		std::string orientation = component[nr.first][nr.second].rotation;
		for(int i = 0; i <= 3; ++i)
		{
			bool ok2 = false;
			if(x == 0 && i == 2)
				continue;
			if(x == 1 && i == 3)
				continue;
			if(x == 2 && i == 0)
				continue;
			if(x == 3 && i == 1)
				continue;

			if(orientation == "up" && i == 2)
				continue;
			if(orientation == "right" && i == 3)
				continue;
			if(orientation == "down" && i == 0)
				continue;
			if(orientation == "left" && i == 1)
				continue;
			if(component[Nexti][Nextj].type == "switch")
				continue;

			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				ok2 = (i == 0 || i == 2) && component[Nexti][Nextj].rotation == "down" ? true : ok2;
				ok2 = (i == 1 || i == 3) && component[Nexti][Nextj].rotation == "right" ? true : ok2;
			}

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				ok2 = i == 0 && component[Nexti][Nextj].rotation == "up" ? true : ok2;
				ok2 = i == 1 && component[Nexti][Nextj].rotation == "right" ? true : ok2;
				ok2 = i == 2 && component[Nexti][Nextj].rotation == "down" ? true : ok2;
				ok2 = i == 3 && component[Nexti][Nextj].rotation == "left" ? true : ok2;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XANDgate:
			if(component[Nexti][Nextj].type == "XANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NORgate:
			if(component[Nexti][Nextj].type == "NORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NANDgate:
			if(component[Nexti][Nextj].type == "NANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
				ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
				ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
				ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
			}

			//Next is wire junction/screen/Wire4way:
			ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;
			ok2 = component[Nexti][Nextj].type == "screen" ? true : ok2;
			ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

			if(ok2 == true)
				sum += daRealFill({Nexti, Nextj}, i), corecte++;
		}
		return sum - corecte + 1;
	}

	//Current is wire4way-Real:
	if(component[nr.first][nr.second].type == "wire4way")
	{
		for(int i = 0; i <= 3; ++i)
		{
			ok2 = false;
			if(i == x)
				continue;
			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				ok2 = component[Nexti][Nextj].rotation == "right" && (i == 1 || i == 3) ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "down" && (i == 0 || i == 2) ? true : ok2;
			}

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				ok2 = component[Nexti][Nextj].rotation == "up" && i == 0 ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "right" && i == 1 ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "down" && i == 2 ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "left" && i == 3 ? true : ok2;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is XANDgate:
			if(component[Nexti][Nextj].type == "XANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NORgate:
			if(component[Nexti][Nextj].type == "NORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is NANDgate:
			if(component[Nexti][Nextj].type == "NANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				ok2 = i == 0 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left") ? true : ok2;
				ok2 = i == 1 && (component[Nexti][Nextj].rotation == "left" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 2 && (component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
				ok2 = i == 3 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "right") ? true : ok2;
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
			}

			//Next is wire4way:
			ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

			//Next is WireJunction:
			ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

			if(ok2 == true)
				sum += daRealFill({Nexti, Nextj}, i) + 1, corecte++;
		}
		return sum - corecte + 1;
	}

	//Current is wireJunction-Real:
	if(component[nr.first][nr.second].type == "wireJunction")
	{
		int i;
		i = x;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
			ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
			ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
			ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is wire4way:
		ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

		//Next is wireJunction:
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		if(ok2 == true)
			return daRealFill({Nexti, Nextj}, i) + 1;
		return 1;
	}

	//Current is ORgate-Real:
	if(component[nr.first][nr.second].type == "ORgate")
	{
		int i;
		std::string var = component[nr.first][nr.second].rotation;
		if(var == "up")
			i = 0;
		if(var == "right")
			i = 1;
		if(var == "down")
			i = 2;
		if(var == "left")
			i = 3;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
			ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
			ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
			ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is wireJunction:
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		//Next is wire4way:
		ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

		if(ok2 == true)
			return daRealFill({Nexti, Nextj}, i) + 1;
		return 1;
	}

	//Current is ANDgate-Real:
	if(component[nr.first][nr.second].type == "ANDgate")
	{
		int i;
		std::string var = component[nr.first][nr.second].rotation;
		if(var == "up")
			i = 0;
		if(var == "right")
			i = 1;
		if(var == "down")
			i = 2;
		if(var == "left")
			i = 3;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
			ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
			ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
			ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is wire4way:
		ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

		//Next is wireJunction:
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		if(ok2 == true)
			return daRealFill({Nexti, Nextj}, i) + 1;
		return 1;
	}

	//Current is XORgate-Real:
	if(component[nr.first][nr.second].type == "XORgate")
	{
		int i;
		std::string var = component[nr.first][nr.second].rotation;
		if(var == "up")
			i = 0;
		if(var == "right")
			i = 1;
		if(var == "down")
			i = 2;
		if(var == "left")
			i = 3;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
			ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
			ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
			ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is wire4way:
		ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

		//Next is wireJunction:
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		if(ok2 == true)
			return daRealFill({Nexti, Nextj}, i) + 1;
		return 1;
	}

	//Current is XANDgate-Real:
	if(component[nr.first][nr.second].type == "XANDgate")
	{
		int i;
		std::string var = component[nr.first][nr.second].rotation;
		if(var == "up")
			i = 0;
		if(var == "right")
			i = 1;
		if(var == "down")
			i = 2;
		if(var == "left")
			i = 3;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
			ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
			ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
			ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is wire4way:
		ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

		//Next is wireJunction:
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		if(ok2 == true)
			return daRealFill({Nexti, Nextj}, i) + 1;
		return 1;
	}

	//Current is NORgate-Real:
	if(component[nr.first][nr.second].type == "NORgate")
	{
		int i;
		std::string var = component[nr.first][nr.second].rotation;
		if(var == "up")
			i = 0;
		if(var == "right")
			i = 1;
		if(var == "down")
			i = 2;
		if(var == "left")
			i = 3;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
			ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
			ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
			ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is wire4way:
		ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

		//Next is wireJunction:
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		if(ok2 == true)
			return daRealFill({Nexti, Nextj}, i) + 1;
		return 1;
	}

	//Current is NANDgate-Real:
	if(component[nr.first][nr.second].type == "NANDgate")
	{
		int i;
		std::string var = component[nr.first][nr.second].rotation;
		if(var == "up")
			i = 0;
		if(var == "right")
			i = 1;
		if(var == "down")
			i = 2;
		if(var == "left")
			i = 3;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
			ok2 = i == 1 && (var == "left" || var == "up") ? true : ok2;
			ok2 = i == 2 && (var == "right" || var == "up") ? true : ok2;
			ok2 = i == 3 && (var == "right" || var == "down") ? true : ok2;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
		}

		//Next is wire4way:
		ok2 = component[Nexti][Nextj].type == "wire4way" ? true : ok2;

		//Next is wireJunction:
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		if(ok2 == true)
			return daRealFill({Nexti, Nextj}, i) + 1;
		return 1;
	}
}

void Level::Fill(std::pair<int, int> nr, int x)
{
	isLoop[nr.first][nr.second]++;
	if(isLoop[nr.first][nr.second] > 1 && component[nr.first][nr.second].type != "wireJunction")
	{
		return;
	}

	if(component[nr.first][nr.second].type == "empty" || nr.first > sizeX || nr.second > sizeY || nr.first < 0 || nr.second < 0)
		return;
	if(component[nr.first][nr.second].type == "switch" && component[nr.first][nr.second].isPowered == false)
		return;

	//Current is Switch-Fill:
	if(component[nr.first][nr.second].type == "switch")
	{
		for(int i = 0; i <= 3; ++i)
		{
			bool ok2 = false;

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var == "up" ? true : ok2;
				ok2 = i == 1 && var == "right" ? true : ok2;
				ok2 = i == 2 && var == "down" ? true : ok2;
				ok2 = i == 3 && var == "left" ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is XANDgate:
			if(component[Nexti][Nextj].type == "XANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is NORgate:
			if(component[Nexti][Nextj].type == "NORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is NANDgate:
			if(component[Nexti][Nextj].type == "NANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				if((component[Nexti][Nextj].rotation == "right" && (i == 1 || i == 3)) || (component[Nexti][Nextj].rotation == "down" && (i == 0 || i == 2)))
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered), ok2 = true;
				else
					continue;
			}

			//Next is wireJunction:
			if(component[Nexti][Nextj].type == "wireJunction")
			{
				if(i == 0 || i == 2)
					component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
				if(i == 1 || i == 3)
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
				ok2 = true;
			}

			//Next is Screen:
			if(component[Nexti][Nextj].type == "screen")
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				if(i == 0)
				{
					if(component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
				if(i == 1)
				{
					if(component[Nexti][Nextj].rotation == "up" || component[Nexti][Nextj].rotation == "left")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
				if(i == 2)
				{
					if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
				if(i == 3)
				{
					if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "down")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			}

			//Next is wire4way:
			if(component[Nexti][Nextj].type == "wire4way")
			{
				ok2 = true;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			}

			if(ok2 == true && Nexti >= 0 && Nexti < sizeX && Nextj >= 0 && Nextj < sizeY && component[Nexti][Nextj].type != "NOTgate" && component[Nexti][Nextj].type != "ORgate" && component[Nexti][Nextj].type != "ANDgate" && component[Nexti][Nextj].type != "XORgate" && component[Nexti][Nextj].type != "XANDgate" && component[Nexti][Nextj].type != "NORgate" && component[Nexti][Nextj].type != "NANDgate")
				Fill({Nexti, Nextj}, i);
		}
	}

	//Current is NOTgate-Fill:
	if(component[nr.first][nr.second].type == "NOTgate")
	{
		int i;
		bool ok2 = false;

		if(component[nr.first][nr.second].rotation == "up")
			i = 0;
		else if(component[nr.first][nr.second].rotation == "right")
			i = 1;
		else if(component[nr.first][nr.second].rotation == "down")
			i = 2;
		else if(component[nr.first][nr.second].rotation == "left")
			i = 3;
		if(component[Nexti][Nextj].type == "screen")
			component[Nexti][Nextj].isPowered = std::max(!component[nr.first][nr.second].isPowered, component[Nexti][Nextj].isPowered);

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var == "up" ? true : ok2;
			ok2 = i == 1 && var == "right" ? true : ok2;
			ok2 = i == 2 && var == "down" ? true : ok2;
			ok2 = i == 3 && var == "left" ? true : ok2;
			if(ok2 == true)
				component[Nexti][Nextj].isPowered = !component[nr.first][nr.second].isPowered;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? !component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			if(component[Nexti][Nextj].rotation == "down" && (i == 0 || i == 2))
				component[Nexti][Nextj].isPowered = std::max(!component[nr.first][nr.second].isPowered, component[Nexti][Nextj].isPowered), ok2 = true;
			if(component[Nexti][Nextj].rotation == "right" && (i == 1 || i == 3))
				component[Nexti][Nextj].isPowered = std::max(!component[nr.first][nr.second].isPowered, component[Nexti][Nextj].isPowered), ok2 = true;
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
			if(ok2 == true)
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, !component[nr.first][nr.second].isPowered);
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, !component[nr.first][nr.second].isPowered), ok2 = true;

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
				component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered2, !component[nr.first][nr.second].isPowered);
			if(i == 1 || i == 3)
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, !component[nr.first][nr.second].isPowered);
			ok2 = true;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			if(i == 0)
			{
				if(component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left")
					component[Nexti][Nextj].isPowered = std::max(!component[nr.first][nr.second].isPowered, component[Nexti][Nextj].isPowered), ok2 = true;
			}
			if(i == 1)
			{
				if(component[Nexti][Nextj].rotation == "up" || component[Nexti][Nextj].rotation == "left")
					component[Nexti][Nextj].isPowered = std::max(!component[nr.first][nr.second].isPowered, component[Nexti][Nextj].isPowered), ok2 = true;
			}
			if(i == 2)
			{
				if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up")
					component[Nexti][Nextj].isPowered = std::max(!component[nr.first][nr.second].isPowered, component[Nexti][Nextj].isPowered), ok2 = true;
			}
			if(i == 3)
			{
				if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "down")
					component[Nexti][Nextj].isPowered = std::max(!component[nr.first][nr.second].isPowered, component[Nexti][Nextj].isPowered), ok2 = true;
			}
		}

		if(ok2 == true && component[Nexti][Nextj].type != "NOTgate" && component[Nexti][Nextj].type != "ORgate" && component[Nexti][Nextj].type != "ANDgate" && component[Nexti][Nextj].type != "XORgate" && component[Nexti][Nextj].type != "XANDgate" && component[Nexti][Nextj].type != "NORgate" && component[Nexti][Nextj].type != "NANDgate")
			Fill({Nexti, Nextj}, i);
	}

	//Current is wire-Fill:
	if(component[nr.first][nr.second].type == "wire")
	{
		int i;
		bool ok2 = false;

		i = component[nr.first][nr.second].rotation == "right" && (x == 1 || x == 3) ? x : i;
		i = component[nr.first][nr.second].rotation == "down" && (x == 0 || x == 2) ? x : i;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire" && component[Nexti][Nextj].rotation == component[nr.first][nr.second].rotation)
			component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered, ok2 = true;

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var == "up" ? true : ok2;
			ok2 = i == 1 && var == "right" ? true : ok2;
			ok2 = i == 2 && var == "down" ? true : ok2;
			ok2 = i == 3 && var == "left" ? true : ok2;
			if(ok2 == true)
				component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is screen:
		if(component[Nexti][Nextj].type == "screen")
			component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
				component[Nexti][Nextj].isPowered2 = component[nr.first][nr.second].isPowered;
			if(i == 1 || i == 3)
				component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
			ok2 = true;
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			if(i == 0)
			{
				if(component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left")
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered, ok2 = true;
			}
			if(i == 1)
			{
				if(component[Nexti][Nextj].rotation == "up" || component[Nexti][Nextj].rotation == "left")
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered, ok2 = true;
			}
			if(i == 2)
			{
				if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up")
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered, ok2 = true;
			}
			if(i == 3)
			{
				if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "down")
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered, ok2 = true;
			}
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var != "up" ? true : ok2;
			ok2 = i == 1 && var != "right" ? true : ok2;
			ok2 = i == 2 && var != "down" ? true : ok2;
			ok2 = i == 3 && var != "left" ? true : ok2;
			if(ok2 == true)
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			ok2 = true;
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
		}

		//Recall:
		if(ok2 == true && component[Nexti][Nextj].type != "empty" && component[Nexti][Nextj].type != "NOTgate" && component[Nexti][Nextj].type != "ORgate"  && component[Nexti][Nextj].type != "ANDgate" && component[Nexti][Nextj].type != "XORgate" && component[Nexti][Nextj].type != "XANDgate" && component[Nexti][Nextj].type != "NORgate" && component[Nexti][Nextj].type != "NANDgate")
			Fill({Nexti, Nextj}, i);
	}

	//Current is wireCorner-Fill:
	if(component[nr.first][nr.second].type == "wireCorner")
	{
		bool ok2 = false;
		int i;

		if(x == 0)
			i = (component[nr.first][nr.second].rotation == "down") ? 1 : 3;
		if(x == 1)
			i = (component[nr.first][nr.second].rotation == "left") ? 2 : 0;
		if(x == 2)
			i = (component[nr.first][nr.second].rotation == "up") ? 3 : 1;
		if(x == 3)
			i = (component[nr.first][nr.second].rotation == "right") ? 0 : 2;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			ok2 = (i == 0 || i == 2) && component[Nexti][Nextj].rotation == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && component[Nexti][Nextj].rotation == "right" ? true : ok2;
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			ok2 = i == 0 && var == "up" ? true : ok2;
			ok2 = i == 1 && var == "right" ? true : ok2;
			ok2 = i == 2 && var == "down" ? true : ok2;
			ok2 = i == 3 && var == "left" ? true : ok2;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			bool ok2 = false;
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left") ? true : ok2;
			ok2 = i == 1 && (component[Nexti][Nextj].rotation == "left" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
			ok2 = i == 2 && (component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up") ? true : ok2;
			ok2 = i == 3 && (component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "right") ? true : ok2;
		}

		//Next is screen/wireJunction:
		ok2 = component[Nexti][Nextj].type == "screen" ? true : ok2;
		ok2 = component[Nexti][Nextj].type == "wireJunction" ? true : ok2;

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			bool ok2 = false;
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
			}
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
				if(Nexti >= 0 && Nexti < sizeX && Nextj >= 0 && Nextj < sizeY && component[Nexti][Nextj].type != "NOTgate" && component[Nexti][Nextj].type != "ORgate" && component[Nexti][Nextj].type != "ANDgate" && component[Nexti][Nextj].type != "XORgate" && component[Nexti][Nextj].type != "XANDgate" && component[Nexti][Nextj].type != "NORgate" && component[Nexti][Nextj].type != "NANDgate")
					Fill({Nexti, Nextj}, i);
				ok2 = false;
			}
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			ok2 = true;
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
		}

		if(ok2 == true)
		{
			if(component[Nexti][Nextj].type != "wireJunction")
			{
				component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
				if(Nexti >= 0 && Nexti < sizeX && Nextj >= 0 && Nextj < sizeY && component[Nexti][Nextj].type != "NOTgate" && component[Nexti][Nextj].type != "ORgate" && component[Nexti][Nextj].type != "ANDgate" && component[Nexti][Nextj].type != "XORgate" && component[Nexti][Nextj].type != "XANDgate" && component[Nexti][Nextj].type != "NORgate" && component[Nexti][Nextj].type != "NANDgate")
					Fill({Nexti, Nextj}, i);
			}
			if(component[Nexti][Nextj].type == "wireJunction")
			{
				if(i == 0 || i == 2)
					component[Nexti][Nextj].isPowered2 = component[nr.first][nr.second].isPowered;
				if(i == 1 || i == 3)
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
				Fill({Nexti, Nextj}, i);
			}
		}
	}

	//Current is wire3way-Fill:
	if(component[nr.first][nr.second].type == "wire3way")
	{
		std::string orientation = component[nr.first][nr.second].rotation;
		for(int i = 0; i <= 3; ++i)
		{
			bool ok2 = false;

			if(x == 0 && i == 2)
				continue;
			if(x == 1 && i == 3)
				continue;
			if(x == 2 && i == 0)
				continue;
			if(x == 3 && i == 1)
				continue;

			if(orientation == "up" && i == 2)
				continue;
			if(orientation == "right" && i == 3)
				continue;
			if(orientation == "down" && i == 0)
				continue;
			if(orientation == "left" && i == 1)
				continue;
			if(component[Nexti][Nextj].type == "switch")
				continue;

			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				ok2 = component[Nexti][Nextj].rotation == "right" && (i == 1 || i == 3) ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "down" && (i == 0 || i == 2) ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
			}

			//Next is wireJunction:
			if(component[Nexti][Nextj].type == "wireJunction")
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 2) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 1 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				ok2 = true;
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && (var == "down" || var == "left") ? true : ok2;
				ok2 = i == 1 && (var == "up" || var == "left") ? true : ok2;
				ok2 = i == 2 && (var == "up" || var == "right") ? true : ok2;
				ok2 = i == 3 && (var == "down" || var == "right") ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && (var != "up") ? true : ok2;
				ok2 = i == 1 && (var != "right") ? true : ok2;
				ok2 = i == 2 && (var != "down") ? true : ok2;
				ok2 = i == 3 && (var != "left") ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
			}

			//Next is wire4way:
			if(component[Nexti][Nextj].type == "wire4way")
			{
				ok2 = true;
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			}

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				ok2 = component[Nexti][Nextj].rotation == "up" && i == 0 ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "right" && i == 1 ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "down" &&  i == 2 ? true : ok2;
				ok2 = component[Nexti][Nextj].rotation == "left" &&  i == 3 ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is XANDgate:
			if(component[Nexti][Nextj].type == "XANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is NORgate:
			if(component[Nexti][Nextj].type == "NORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is NANDgate:
			if(component[Nexti][Nextj].type == "NANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is screen:
			if(component[Nexti][Nextj].type == "screen")
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			if(ok2 == true && Nexti >= 0 && Nexti < sizeX && Nextj >= 0 && Nextj < sizeY && component[Nexti][Nextj].type != "NOTgate" && component[Nexti][Nextj].type != "ORgate" && component[Nexti][Nextj].type != "ANDgate" && component[Nexti][Nextj].type != "XORgate" && component[Nexti][Nextj].type != "XANDgate" && component[Nexti][Nextj].type != "NORgate" && component[Nexti][Nextj].type != "NANDgate")
				Fill({Nexti, Nextj}, i);
		}
	}

	//Current is wire4way-Fill:
	if(component[nr.first][nr.second].type == "wire4way")
	{
		for(int i = 0; i <= 3; ++i)
		{
			bool ok2 = false;
			if(i == 0 && x == 2)
				continue;
			if(i == 1 && x == 3)
				continue;
			if(i == 2 && x == 0)
				continue;
			if(i == 3 && x == 1)
				continue;

			//Next is NOTgate:
			if(component[Nexti][Nextj].type == "NOTgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var == "up" ? true : ok2;
				ok2 = i == 1 && var == "right" ? true : ok2;
				ok2 = i == 2 && var == "down" ? true : ok2;
				ok2 = i == 3 && var == "left" ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = component[nr.first][nr.second].isPowered;
			}

			//Next is ORgate:
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is ANDgate:
			if(component[Nexti][Nextj].type == "ANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is XORgate:
			if(component[Nexti][Nextj].type == "XORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is XANDgate:
			if(component[Nexti][Nextj].type == "XANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is NORgate:
			if(component[Nexti][Nextj].type == "NORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is NANDgate:
			if(component[Nexti][Nextj].type == "NANDgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
				if(ok2 == true)
				{
					component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered2;
					component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
				}
			}

			//Next is wire:
			if(component[Nexti][Nextj].type == "wire")
			{
				if((component[Nexti][Nextj].rotation == "right" && (i == 1 || i == 3)) || (component[Nexti][Nextj].rotation == "down" && (i == 0 || i == 2)))
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered), ok2 = true;
				else
					continue;
			}

			//Next is wireJunction:
			if(component[Nexti][Nextj].type == "wireJunction")
			{
				if(i == 0 || i == 2)
					component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
				if(i == 1 || i == 3)
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
				ok2 = true;
			}

			//Next is Screen:
			if(component[Nexti][Nextj].type == "screen")
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			}

			//Next is wireCorner:
			if(component[Nexti][Nextj].type == "wireCorner")
			{
				if(i == 0)
				{
					if(component[Nexti][Nextj].rotation == "down" || component[Nexti][Nextj].rotation == "left")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
				if(i == 1)
				{
					if(component[Nexti][Nextj].rotation == "up" || component[Nexti][Nextj].rotation == "left")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
				if(i == 2)
				{
					if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "up")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
				if(i == 3)
				{
					if(component[Nexti][Nextj].rotation == "right" || component[Nexti][Nextj].rotation == "down")
						component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
					ok2 = true;
				}
			}

			//Next is wire3way:
			if(component[Nexti][Nextj].type == "wire3way")
			{
				std::string var = component[Nexti][Nextj].rotation;
				ok2 = i == 0 && var != "up" ? true : ok2;
				ok2 = i == 1 && var != "right" ? true : ok2;
				ok2 = i == 2 && var != "down" ? true : ok2;
				ok2 = i == 3 && var != "left" ? true : ok2;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			}

			//Next is wire4way:
			if(component[Nexti][Nextj].type == "wire4way")
			{
				ok2 = true;
				if(ok2 == true)
					component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, component[nr.first][nr.second].isPowered);
			}

			if(ok2 == true && Nexti >= 0 && Nexti < sizeX && Nextj >= 0 && Nextj < sizeY && component[Nexti][Nextj].type != "NOTgate" && component[Nexti][Nextj].type != "ORgate" && component[Nexti][Nextj].type != "ANDgate" && component[Nexti][Nextj].type != "XORgate" && component[Nexti][Nextj].type != "XANDgate" && component[Nexti][Nextj].type != "NORgate" && component[Nexti][Nextj].type != "NANDgate")
				Fill({Nexti, Nextj}, i);
		}
	}

	//Current is wireJunction-Fill:
	if(component[nr.first][nr.second].type == "wireJunction")
	{
		int i = x;
		bool var;
		std::string orientation = component[Nexti][Nextj].rotation;
		bool ok2 = false;

		var = (i == 0 || i == 2) ? component[nr.first][nr.second].isPowered2 : var;
		var = (i == 1 || i == 3) ? component[nr.first][nr.second].isPowered : var;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			ok2 = (i == 0 || i == 2) && orientation == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && orientation == "right" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var;
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			ok2 = orientation == "up" && i == 0 ? true : ok2;
			ok2 = orientation == "right" && i == 1 ? true : ok2;
			ok2 = orientation == "down" &&  i == 2 ? true : ok2;
			ok2 = orientation == "left" &&  i == 3 ? true : ok2;
			if(ok2 == true)
				component[Nexti][Nextj].isPowered = var;
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				bool variabila = (x == 0 || x == 2) ? component[nr.first][nr.second].isPowered2 : component[nr.first][nr.second].isPowered;
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? variabila : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? variabila : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				bool variabila = (x == 0 || x == 2) ? component[nr.first][nr.second].isPowered2 : component[nr.first][nr.second].isPowered;
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? variabila : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? variabila : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				bool variabila = (x == 0 || x == 2) ? component[nr.first][nr.second].isPowered2 : component[nr.first][nr.second].isPowered;
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? variabila : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? variabila : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				bool variabila = (x == 0 || x == 2) ? component[nr.first][nr.second].isPowered2 : component[nr.first][nr.second].isPowered;
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? variabila : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? variabila : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				bool variabila = (x == 0 || x == 2) ? component[nr.first][nr.second].isPowered2 : component[nr.first][nr.second].isPowered;
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? variabila : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? variabila : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				bool variabila = (x == 0 || x == 2) ? component[nr.first][nr.second].isPowered2 : component[nr.first][nr.second].isPowered;
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? variabila : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? variabila : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is wireCorner:
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (orientation == "down" || orientation == "left") ? true : ok2;
			ok2 = i == 1 && (orientation == "up" || orientation == "left") ? true : ok2;
			ok2 = i == 2 && (orientation == "right" || orientation == "up") ? true : ok2;
			ok2 = i == 3 && (orientation == "down" || orientation == "right") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var;
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire3way:
		if(component[Nexti][Nextj].type == "wire3way")
		{
			ok2 = i == 0 && (orientation != "up") ? true : ok2;
			ok2 = i == 1 && (orientation != "right") ? true : ok2;
			ok2 = i == 2 && (orientation != "down") ? true : ok2;
			ok2 = i == 3 && (orientation != "left") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var;
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire4way;
		if(component[Nexti][Nextj].type == "wire4way")
		{
			component[Nexti][Nextj].isPowered = var;
			Fill({Nexti, Nextj}, i);
		}
		//Next is screen:
		if(component[Nexti][Nextj].type == "screen")
		{
			component[Nexti][Nextj].isPowered = var;
		}

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			component[Nexti][Nextj].isPowered2 = (i == 0 || i == 2) ? component[nr.first][nr.second].isPowered2 : component[Nexti][Nextj].isPowered2;
			component[Nexti][Nextj].isPowered = (i == 1 || i == 3) ? component[nr.first][nr.second].isPowered : component[Nexti][Nextj].isPowered;
			Fill({Nexti, Nextj}, i);
		}
	}

	//Current is ORgate-Fill:
	if(component[nr.first][nr.second].type == "ORgate")
	{
		std::string orientation = component[nr.first][nr.second].rotation;
		bool var1 = std::max(component[nr.first][nr.second].isPowered2, component[nr.first][nr.second].isPowered);
		bool ok2 = false;
		int i;
		i = orientation == "up" ? 0 : i;
		i = orientation == "right" ? 1 : i;
		i = orientation == "down" ? 2 : i;
		i = orientation == "left" ? 3 : i;
		orientation = component[Nexti][Nextj].rotation;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std:: string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1), Fill({Nexti, Nextj}, i);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wireCorner
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (orientation == "down" || orientation == "left") ? true : ok2;
			ok2 = i == 1 && (orientation == "up" || orientation == "left") ? true : ok2;
			ok2 = i == 2 && (orientation == "right" || orientation == "up") ? true : ok2;
			ok2 = i == 3 && (orientation == "down" || orientation == "right") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire3way
		if(component[Nexti][Nextj].type == "wire3way")
		{
			ok2 = i == 0 && orientation != "up" ? true : ok2;
			ok2 = i == 1 && orientation != "right" ? true : ok2;
			ok2 = i == 2 && orientation != "down" ? true : ok2;
			ok2 = i == 3 && orientation != "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			Fill({Nexti, Nextj}, i);
		}

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
			{
				component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered2, var1);
			}
			else if(i == 1 || i == 3)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			}
			Fill({Nexti, Nextj}, i);
		}

		//Next is screen:
		component[Nexti][Nextj].isPowered = component[Nexti][Nextj].type == "screen" ? std::max(component[Nexti][Nextj].isPowered, var1) : component[Nexti][Nextj].isPowered;

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			ok2 = i == 0 && orientation == "up" ? true : ok2;
			ok2 = i == 1 && orientation == "right" ? true : ok2;
			ok2 = i == 2 && orientation == "down" ? true : ok2;
			ok2 = i == 3 && orientation == "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var1;
			}
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}
	}

	//Current is XORgate-Fill:
	if(component[nr.first][nr.second].type == "XORgate")
	{
		std::string orientation = component[nr.first][nr.second].rotation;
		bool var1 = component[nr.first][nr.second].isPowered2 != component[nr.first][nr.second].isPowered;
		bool ok2 = false;
		int i;
		i = orientation == "up" ? 0 : i;
		i = orientation == "right" ? 1 : i;
		i = orientation == "down" ? 2 : i;
		i = orientation == "left" ? 3 : i;
		orientation = component[Nexti][Nextj].rotation;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std:: string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1), Fill({Nexti, Nextj}, i);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wireCorner
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (orientation == "down" || orientation == "left") ? true : ok2;
			ok2 = i == 1 && (orientation == "up" || orientation == "left") ? true : ok2;
			ok2 = i == 2 && (orientation == "right" || orientation == "up") ? true : ok2;
			ok2 = i == 3 && (orientation == "down" || orientation == "right") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire3way
		if(component[Nexti][Nextj].type == "wire3way")
		{
			ok2 = i == 0 && orientation != "up" ? true : ok2;
			ok2 = i == 1 && orientation != "right" ? true : ok2;
			ok2 = i == 2 && orientation != "down" ? true : ok2;
			ok2 = i == 3 && orientation != "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			Fill({Nexti, Nextj}, i);
		}

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
			{
				component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered2, var1);
			}
			else if(i == 1 || i == 3)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			}
			Fill({Nexti, Nextj}, i);
		}

		//Next is screen:
		component[Nexti][Nextj].isPowered = component[Nexti][Nextj].type == "screen" ? std::max(component[Nexti][Nextj].isPowered, var1) : component[Nexti][Nextj].isPowered;

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			ok2 = i == 0 && orientation == "up" ? true : ok2;
			ok2 = i == 1 && orientation == "right" ? true : ok2;
			ok2 = i == 2 && orientation == "down" ? true : ok2;
			ok2 = i == 3 && orientation == "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var1;
			}
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			bool ok2 = false;
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}
	}

	//Current is XANDgate-Fill:
	if(component[nr.first][nr.second].type == "XANDgate")
	{
		std::string orientation = component[nr.first][nr.second].rotation;
		bool var1 = component[nr.first][nr.second].isPowered2 == component[nr.first][nr.second].isPowered;
		bool ok2 = false;
		int i;
		i = orientation == "up" ? 0 : i;
		i = orientation == "right" ? 1 : i;
		i = orientation == "down" ? 2 : i;
		i = orientation == "left" ? 3 : i;
		orientation = component[Nexti][Nextj].rotation;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std:: string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			if(ok2 == true)
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1), Fill({Nexti, Nextj}, i);
		}

		//Next is wireCorner
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (orientation == "down" || orientation == "left") ? true : ok2;
			ok2 = i == 1 && (orientation == "up" || orientation == "left") ? true : ok2;
			ok2 = i == 2 && (orientation == "right" || orientation == "up") ? true : ok2;
			ok2 = i == 3 && (orientation == "down" || orientation == "right") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire3way
		if(component[Nexti][Nextj].type == "wire3way")
		{
			ok2 = i == 0 && orientation != "up" ? true : ok2;
			ok2 = i == 1 && orientation != "right" ? true : ok2;
			ok2 = i == 2 && orientation != "down" ? true : ok2;
			ok2 = i == 3 && orientation != "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			Fill({Nexti, Nextj}, i);
		}

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
			{
				component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered2, var1);
			}
			else if(i == 1 || i == 3)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			}
			Fill({Nexti, Nextj}, i);
		}

		//Next is screen:
		component[Nexti][Nextj].isPowered = component[Nexti][Nextj].type == "screen" ? std::max(component[Nexti][Nextj].isPowered, var1) : component[Nexti][Nextj].isPowered;

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			ok2 = i == 0 && orientation == "up" ? true : ok2;
			ok2 = i == 1 && orientation == "right" ? true : ok2;
			ok2 = i == 2 && orientation == "down" ? true : ok2;
			ok2 = i == 3 && orientation == "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var1;
			}
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			bool ok2 = false;
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}
	}

	//Current is NORgate-Fill:
	if(component[nr.first][nr.second].type == "NORgate")
	{
		std::string orientation = component[nr.first][nr.second].rotation;
		bool var1 = std::max(component[nr.first][nr.second].isPowered2, component[nr.first][nr.second].isPowered);
		var1 = !var1;
		bool ok2 = false;
		int i;
		i = orientation == "up" ? 0 : i;
		i = orientation == "right" ? 1 : i;
		i = orientation == "down" ? 2 : i;
		i = orientation == "left" ? 3 : i;
		orientation = component[Nexti][Nextj].rotation;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std:: string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1), Fill({Nexti, Nextj}, i);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wireCorner
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (orientation == "down" || orientation == "left") ? true : ok2;
			ok2 = i == 1 && (orientation == "up" || orientation == "left") ? true : ok2;
			ok2 = i == 2 && (orientation == "right" || orientation == "up") ? true : ok2;
			ok2 = i == 3 && (orientation == "down" || orientation == "right") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire3way
		if(component[Nexti][Nextj].type == "wire3way")
		{
			ok2 = i == 0 && orientation != "up" ? true : ok2;
			ok2 = i == 1 && orientation != "right" ? true : ok2;
			ok2 = i == 2 && orientation != "down" ? true : ok2;
			ok2 = i == 3 && orientation != "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			Fill({Nexti, Nextj}, i);
		}

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
			{
				component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered2, var1);
			}
			else if(i == 1 || i == 3)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			}
			Fill({Nexti, Nextj}, i);
		}

		//Next is screen:
		component[Nexti][Nextj].isPowered = component[Nexti][Nextj].type == "screen" ? std::max(component[Nexti][Nextj].isPowered, var1) : component[Nexti][Nextj].isPowered;

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			ok2 = i == 0 && orientation == "up" ? true : ok2;
			ok2 = i == 1 && orientation == "right" ? true : ok2;
			ok2 = i == 2 && orientation == "down" ? true : ok2;
			ok2 = i == 3 && orientation == "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var1;
			}
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			bool ok2 = false;
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}
	}

	//Current is NANDgate-Fill:
	if(component[nr.first][nr.second].type == "NANDgate")
	{
		std::string orientation = component[nr.first][nr.second].rotation;
		bool var1 = component[nr.first][nr.second].isPowered2 && component[nr.first][nr.second].isPowered;
		var1 = !var1;
		bool ok2 = false;
		int i;
		i = orientation == "up" ? 0 : i;
		i = orientation == "right" ? 1 : i;
		i = orientation == "down" ? 2 : i;
		i = orientation == "left" ? 3 : i;
		orientation = component[Nexti][Nextj].rotation;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std:: string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1), Fill({Nexti, Nextj}, i);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wireCorner
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (orientation == "down" || orientation == "left") ? true : ok2;
			ok2 = i == 1 && (orientation == "up" || orientation == "left") ? true : ok2;
			ok2 = i == 2 && (orientation == "right" || orientation == "up") ? true : ok2;
			ok2 = i == 3 && (orientation == "down" || orientation == "right") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire3way
		if(component[Nexti][Nextj].type == "wire3way")
		{
			ok2 = i == 0 && orientation != "up" ? true : ok2;
			ok2 = i == 1 && orientation != "right" ? true : ok2;
			ok2 = i == 2 && orientation != "down" ? true : ok2;
			ok2 = i == 3 && orientation != "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			Fill({Nexti, Nextj}, i);
		}

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
			{
				component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered2, var1);
			}
			else if(i == 1 || i == 3)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			}
			Fill({Nexti, Nextj}, i);
		}

		//Next is screen:
		component[Nexti][Nextj].isPowered = component[Nexti][Nextj].type == "screen" ? std::max(component[Nexti][Nextj].isPowered, var1) : component[Nexti][Nextj].isPowered;

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			ok2 = i == 0 && orientation == "up" ? true : ok2;
			ok2 = i == 1 && orientation == "right" ? true : ok2;
			ok2 = i == 2 && orientation == "down" ? true : ok2;
			ok2 = i == 3 && orientation == "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var1;
			}
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			bool ok2 = false;
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}
	}

	//Current is ANDgate-Fill:
	if(component[nr.first][nr.second].type == "ANDgate")
	{
		std::string orientation = component[nr.first][nr.second].rotation;
		bool var1 = component[nr.first][nr.second].isPowered2 && component[nr.first][nr.second].isPowered;
		bool ok2 = false;
		int i;
		i = orientation == "up" ? 0 : i;
		i = orientation == "right" ? 1 : i;
		i = orientation == "down" ? 2 : i;
		i = orientation == "left" ? 3 : i;
		orientation = component[Nexti][Nextj].rotation;

		//Next is wire:
		if(component[Nexti][Nextj].type == "wire")
		{
			std:: string var = component[Nexti][Nextj].rotation;
			ok2 = (i == 0 || i == 2) && var == "down" ? true : ok2;
			ok2 = (i == 1 || i == 3) && var == "right" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1), Fill({Nexti, Nextj}, i);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wireCorner
		if(component[Nexti][Nextj].type == "wireCorner")
		{
			ok2 = i == 0 && (orientation == "down" || orientation == "left") ? true : ok2;
			ok2 = i == 1 && (orientation == "up" || orientation == "left") ? true : ok2;
			ok2 = i == 2 && (orientation == "right" || orientation == "up") ? true : ok2;
			ok2 = i == 3 && (orientation == "down" || orientation == "right") ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire3way
		if(component[Nexti][Nextj].type == "wire3way")
		{
			ok2 = i == 0 && orientation != "up" ? true : ok2;
			ok2 = i == 1 && orientation != "right" ? true : ok2;
			ok2 = i == 2 && orientation != "down" ? true : ok2;
			ok2 = i == 3 && orientation != "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
				Fill({Nexti, Nextj}, i);
			}
		}

		//Next is wire4way:
		if(component[Nexti][Nextj].type == "wire4way")
		{
			component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			Fill({Nexti, Nextj}, i);
		}

		//Next is wireJunction:
		if(component[Nexti][Nextj].type == "wireJunction")
		{
			if(i == 0 || i == 2)
			{
				component[Nexti][Nextj].isPowered2 = std::max(component[Nexti][Nextj].isPowered2, var1);
			}
			else if(i == 1 || i == 3)
			{
				component[Nexti][Nextj].isPowered = std::max(component[Nexti][Nextj].isPowered, var1);
			}
			Fill({Nexti, Nextj}, i);
		}

		//Next is screen:
		component[Nexti][Nextj].isPowered = component[Nexti][Nextj].type == "screen" ? std::max(component[Nexti][Nextj].isPowered, var1) : component[Nexti][Nextj].isPowered;

		//Next is NOTgate:
		if(component[Nexti][Nextj].type == "NOTgate")
		{
			ok2 = i == 0 && orientation == "up" ? true : ok2;
			ok2 = i == 1 && orientation == "right" ? true : ok2;
			ok2 = i == 2 && orientation == "down" ? true : ok2;
			ok2 = i == 3 && orientation == "left" ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered = var1;
			}
		}

		//Next is ORgate:
		if(component[Nexti][Nextj].type == "ORgate")
		{
			bool ok2 = false;
			if(component[Nexti][Nextj].type == "ORgate")
			{
				std::string var = component[Nexti][Nextj].rotation;
				bool isVertical = false;
				isVertical = var == "up" || var == "down" ? true : false;
				ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
				ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			}
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is ANDgate:
		if(component[Nexti][Nextj].type == "ANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XORgate:
		if(component[Nexti][Nextj].type == "XORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is XANDgate:
		if(component[Nexti][Nextj].type == "XANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NORgate:
		if(component[Nexti][Nextj].type == "NORgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}

		//Next is NANDgate:
		if(component[Nexti][Nextj].type == "NANDgate")
		{
			bool ok2 = false;
			std::string var = component[Nexti][Nextj].rotation;
			bool isVertical = false;
			isVertical = var == "up" || var == "down" ? true : false;
			ok2 = (i == 0 || i == 2) && isVertical == false ? true : ok2;
			ok2 = (i == 1 || i == 3) && isVertical == true ? true : ok2;
			if(ok2 == true)
			{
				component[Nexti][Nextj].isPowered2 = (i == 0 || i == 1) ? var1 : component[Nexti][Nextj].isPowered2;
				component[Nexti][Nextj].isPowered = (i == 2 || i == 3) ? var1 : component[Nexti][Nextj].isPowered;
			}
		}
	}
}

void Level::clearMap()
{
	for(int x = 0; x < sizeX; x++)
		for(int y = 0; y < sizeY; y++)
			if(component[x][y].type != "switch")
			{
				component[x][y].isPowered = false;
				component[x][y].isPowered2 = false;
			}

}

int Level::update()
{
	std::fill(v, v + __MaxN, 0);
	for(int i = 0; i < comp.size(); ++i)
	{
		for(int x = 0; x < sizeX; x++)
		{
			for(int y = 0; y < sizeY; y++)
			{
				isVisited[x][y] = 0;
				isLoop[x][y] = 0;
				if(component[x][y].type != "switch" && component[x][y].type != "NOTgate")
				{
					component[x][y].isPowered = false;
					component[x][y].isPowered2 = false;
				}
			}
		}
		v[i] = daRealFill(comp[i], 0);
	}

	for(int x = 0; x < sizeX; x++)
	{
		for(int y = 0; y < sizeY; y++)
		{
			isVisited[x][y] = 0;
		}
	}

	for(int i = 0; i < comp.size(); ++i)
	{
		for(int j = i + 1; j < comp.size(); ++j)
		{
			if(v[i] < v[j])
			{
				std::swap(v[i], v[j]);
				std::swap(comp[i], comp[j]);
			}
		}
	}
	for(int i = 0; i < comp.size(); ++i)
	{
		for(int x = 0; x < sizeX; ++x)
			for(int y = 0; y < sizeY; ++y)
			{
				isVisited[x][y] = 0;
				isLoop[x][y] = 0;
			}
		Fill({comp[i].first, comp[i].second}, 0);
	}
}
