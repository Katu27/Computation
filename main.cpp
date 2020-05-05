#include "Level.h"
#include "Component.h"

#include <thread>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <cmath>

//MAINICON ICON "icon.png"

using namespace sf;

std::string versionNumber = "alpha-2.0.0";

//Window Variables:
int windowSizeX = 1280;
int windowSizeY = 720;
RenderWindow window;
Event windowEvent;
bool Fullscreen = false;

int errorCode;

//Frame timer:
Clock frameClock;
Clock levelUpdateClock;
Clock blockPlacementClock;
float deltaTime;

int sizeX = 51;
int sizeY = 51;

int ok_2;
int ok_3;

Music bgMusic;
Sound placeSound;
Sound removeSound;
Sound switchSound;

SoundBuffer placeSoundBuf;
SoundBuffer removeSoundBuf;
SoundBuffer switchSoundBuf;

//Camera:
float cameraCenterX = 0.0f;
float cameraCenterY = 0.0f;
float cameraCornerX = 0.0f;
float cameraCornerY = 0.0f;
float cameraSizeX = windowSizeX;
float cameraSizeY = windowSizeY;
float cameraSizeMaxX = 15360;
float cameraSizeMaxY = 8640;
float cameraSizeMinX = 256;
float cameraSizeMinY = 144;
float cameraSpeed = 1.0f;
float cameraZoomSpeed = 0.001f;
float cameraZoom = 1.0f;
View camera(FloatRect(Vector2f(cameraCenterX, cameraCenterY), Vector2f(cameraSizeX, cameraSizeY)));

//Sound::
float musicVolume;
float sfxVolume;

Font font;
Font logoFont;

//LevelUpdate:
int loops;

//Textures:
Texture floorTexture;
Texture emptyTexture;
Texture switchOnTexture;
Texture switchOffTexture;
Texture screenOnTexture;
Texture screenOffTexture;

//Wire Texture:
Texture wireOnrightTexture;
Texture wireOndownTexture;
Texture wireOffrightTexture;
Texture wireOffdownTexture;

//WireCorner Texture:
Texture wireCornerOffrightTexture;
Texture wireCornerOffdownTexture;
Texture wireCornerOffleftTexture;
Texture wireCornerOffupTexture;
Texture wireCornerOnrightTexture;
Texture wireCornerOndownTexture;
Texture wireCornerOnleftTexture;
Texture wireCornerOnupTexture;

//Wire3way Texture:
Texture wire3wayOffrightTexture;
Texture wire3wayOffdownTexture;
Texture wire3wayOffleftTexture;
Texture wire3wayOffupTexture;
Texture wire3wayOnrightTexture;
Texture wire3wayOndownTexture;
Texture wire3wayOnleftTexture;
Texture wire3wayOnupTexture;

//Wire4way Texture:
Texture wire4wayOffrightTexture;
Texture wire4wayOnrightTexture;

//WireJunction:
Texture wireJunctionAllpowered;
Texture wireJunctionAlloff;
Texture wireJunctionVerticalpowered;
Texture wireJunctionHorizontalpowered;


//Gates Textures:
Texture NOTrightTexture;
Texture NOTdownTexture;
Texture NOTleftTexture;
Texture NOTupTexture;
Texture ORrightTexture;
Texture ORupTexture;
Texture ORdownTexture;
Texture ORleftTexture;
Texture ANDupTexture;
Texture ANDrightTexture;
Texture ANDdownTexture;
Texture ANDleftTexture;
Texture XORupTexture;
Texture XORrightTexture;
Texture XORdownTexture;
Texture XORleftTexture;
Texture XANDupTexture;
Texture XANDrightTexture;
Texture XANDdownTexture;
Texture XANDleftTexture;
Texture NORupTexture;
Texture NORrightTexture;
Texture NORdownTexture;
Texture NORleftTexture;
Texture NANDupTexture;
Texture NANDrightTexture;
Texture NANDdownTexture;
Texture NANDleftTexture;


//Cursor:
Vector2i mPos;
float cursorX, cursorY;
bool drawCursor = true;
Component cursorComponent;
RectangleShape cursorHitbox;

Text cursorText;
std::string cursorString = "hey";
Font cursorFont;

int windowModeSizeX, windowModeSizeY;
float levelUpdateDelay;
std::string texturePath;

bool is_slot1 = false;
bool is_slot2 = false;
bool is_slot3 = false;
bool is_slot4 = false;

int mapSize = -1;

int saveSlot = 0;

Image icon;

int saveConfig()
{
    std::ofstream fout("config.txt");

    fout << Fullscreen << std::endl;
    fout << windowModeSizeX << std::endl;
    fout << windowModeSizeY << std::endl;
    fout << levelUpdateDelay << std::endl;
    fout << musicVolume << std::endl;
    fout << sfxVolume << std::endl;
    fout << texturePath << std::endl;

    fout.close();
}

int loadConfig()
{
    std::ifstream fin("config.txt");

    fin >> Fullscreen;
    fin >> windowModeSizeX;
    fin >> windowModeSizeY;
    fin >> levelUpdateDelay;
    fin >> musicVolume;
    fin >> sfxVolume;
    fin >> texturePath;

    fin.close();

    icon.loadFromFile("icon.png");
}

void checkMap(std::string filename)
{
    std::string type, rotation;
    int mapSizeTemp;

    std::ifstream fin ("map/" + filename);
    fin >> mapSizeTemp;
    for (int x = 1; x <= mapSizeTemp; x++)
        for (int y = 1; y <= mapSizeTemp; y++)
        {
            fin>>type>>rotation;
            if(type != "empty")
            {
                if(filename == "slot1.map")
                    is_slot1 = true;
                if(filename == "slot2.map")
                    is_slot2 = true;
                if(filename == "slot3.map")
                    is_slot3 = true;
                if(filename == "slot4.map")
                    is_slot4 = true;

                return;
            }
        }
    fin.close();
}

void deleteMap(std::string filename)
{
    int mapSizeTemp;
    std::ofstream fout ("map/" + filename);
    fout << 25;
    for (int x = 1; x <= 25; x++)
        for (int y = 1; y <= 25; y++)
            fout << "empty" <<" "<< "right" << std::endl;
    fout.close();
}

void loadMapSize(std::string filename)
{
    std::ifstream fin ("map/" + filename);
    fin >> mapSize;
}

void loadMap(std::string filename, Level &level)
{
    std::ifstream fin ("map/" + filename);
    fin >> mapSize;
    for (int x = 1; x <= mapSize; x++)
        for (int y = 1; y <= mapSize; y++)
        {
            fin >> level.component[x][y].type >> level.component[x][y].rotation;
            if (level.component[x][y].type != "empty" && level.component[x][y].type != "wire" && level.component[x][y].type != "wireCorner" && level.component[x][y].type != "wire3way" && level.component[x][y].type != "wireJunction")
                level.comp.push_back ({x, y});
        }
    fin.close();
}

void saveMap(std::string filename, Level &level)
{
    std::ofstream fout ("map/" + filename);
    fout << mapSize;
    for (int x = 1; x <= mapSize; x++)
        for (int y = 1; y <= mapSize; y++)
            fout << level.component[x][y].type <<" "<< level.component[x][y].rotation << std::endl;
    fout.close();
}

void createMap(std::string filename)
{
    std::ofstream fout ("map/" + filename);
    fout << mapSize;
    for (int x = 1; x <= mapSize; x++)
        for (int y = 1; y <= mapSize; y++)
            fout << "empty" <<" "<< "right" << std::endl;
}

int mainMenu()
{
    loadConfig();

    //MapChecking:
    checkMap("slot1.map");
    checkMap("slot2.map");
    checkMap("slot3.map");
    checkMap("slot4.map");

    //OpenFirstMenu:
    RenderWindow menu;
    Event menuEvent;
    Clock inputBuffer;
    float inputDelay = 100;

    //Temp variables config:
    bool tempFullscreen = Fullscreen;
    int tempWindowModeSizeX = windowModeSizeX;
    int tempWindowModeSizeY = windowModeSizeY;
    float tempMusicVolume = musicVolume;
    float tempSfxVolume = sfxVolume;
    float tempLevelUpdateDelay = levelUpdateDelay;
    std::string tempTexturePath = texturePath;

    int menuID = 0;

    Sprite bgSprite;
    Texture bgTexture;

    Start:

    bgTexture.loadFromFile("assets/" + texturePath + "/Menu/floor.png");
    bgSprite.setTexture(bgTexture);


    font.loadFromFile("assets/" + texturePath + "/font.ttf");


    logoFont.loadFromFile("assets/" + texturePath + "/logoFont.ttf");
    bgMusic.setVolume(musicVolume*100);

    if(Fullscreen == true)
    {
        menu.create(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Computation", Style::Fullscreen);
        menu.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        menu.setFramerateLimit(100);

        //Logo
        Text logoText;
        logoText.setFont(logoFont);
        logoText.setString("COMPUTATION");
        logoText.setCharacterSize((VideoMode::getDesktopMode().width*200)/1920);
        logoText.setFillColor(Color(255,255,255));
        logoText.setOutlineColor(Color(0,0,0));
        logoText.setOutlineThickness((VideoMode::getDesktopMode().width*15)/1920);
        FloatRect logoTextRect = logoText.getLocalBounds();
        logoText.setOrigin(logoTextRect.left+logoTextRect.width/2.0f,logoTextRect.top+logoTextRect.height/2.0f);
        logoText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/4));

        //info:
        Text infoText;
        infoText.setFont(font);
        infoText.setString("Made by Ene Alexandru & Alexe Paul\nVersion: " + versionNumber);
        infoText.setCharacterSize((VideoMode::getDesktopMode().width*15)/1920);
        infoText.setFillColor(Color(255,255,255));
        infoText.setOutlineColor(Color(0,0,0));
        infoText.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        infoText.setPosition(Vector2f(0,0));

        //Play button:
        RectangleShape playButton;
        playButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        playButton.setOrigin(Vector2f(playButton.getLocalBounds().width/2, playButton.getGlobalBounds().height/2));
        playButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));
        playButton.setFillColor(Color(255, 182, 64));
        playButton.setOutlineColor(Color(255,255,255));
        playButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text playButtonText;
        playButtonText.setFont(font);
        playButtonText.setString("PLAY");
        playButtonText.setCharacterSize((VideoMode::getDesktopMode().width*100)/1920);
        playButtonText.setFillColor(Color(0,0,0));
        FloatRect playButtonTextRect = playButtonText.getLocalBounds();
        playButtonText.setOrigin(playButtonTextRect.left+playButtonTextRect.width/2.0f,playButtonTextRect.top+playButtonTextRect.height/2.0f);
        playButtonText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));

        //PlayBackButton:
        RectangleShape playBackButton;
        playBackButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        playBackButton.setOrigin(Vector2f(playBackButton.getLocalBounds().width/2, playBackButton.getGlobalBounds().height/2));
        playBackButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + playBackButton.getSize().x*1.2*2, VideoMode::getDesktopMode().height/2 + playBackButton.getSize().y*1.2*3));
        playBackButton.setFillColor(Color(222, 55, 55));
        playBackButton.setOutlineColor(Color(255,255,255));
        playBackButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text playBackText;
        playBackText.setFont(font);
        playBackText.setString("Back");
        playBackText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        playBackText.setFillColor(Color(255,255,255));
        FloatRect playBackTextRect = playBackText.getLocalBounds();
        playBackText.setOrigin(playBackTextRect.left+playBackTextRect.width/2.0f,playBackTextRect.top+playBackTextRect.height/2.0f);
        playBackText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + playBackButton.getSize().x*1.2*2, VideoMode::getDesktopMode().height/2 + playBackButton.getSize().y*1.2*3));

        //Slot1:
        RectangleShape slot1;
        slot1.setSize(Vector2f(VideoMode::getDesktopMode().width/4, VideoMode::getDesktopMode().height / 10));
        slot1.setPosition(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height/10));
        slot1.setOutlineColor(Color(255,255,255));
        slot1.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot1Text;
        slot1Text.setFont(font);
        slot1Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot1Text.setFillColor(Color(255,255,255));
        if(is_slot1 == true)
        {
            slot1.setFillColor(Color(96, 184, 84));
            slot1Text.setString("Save slot #1");
        }
        else if(is_slot1 != true)
        {
            slot1.setFillColor(Color(113, 127, 128));
            slot1Text.setString("Empty");
        }
        FloatRect slot1TextRect = slot1Text.getLocalBounds();
        slot1Text.setOrigin(slot1TextRect.left+slot1TextRect.width/2.0f,slot1TextRect.top+slot1TextRect.height/2.0f);
        slot1Text.setPosition(slot1.getPosition().x+slot1.getSize().x/2, slot1.getPosition().y+slot1.getSize().y/2);

        //Slot1 delete/crate
        RectangleShape slot1DC;
        slot1DC.setSize(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height / 10));
        slot1DC.setPosition(Vector2f(VideoMode::getDesktopMode().width/10 + slot1.getSize().x*1.1, VideoMode::getDesktopMode().height/10));
        slot1DC.setOutlineColor(Color(255,255,255));
        slot1DC.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot1DCText;
        slot1DCText.setFont(font);
        slot1DCText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot1DCText.setFillColor(Color(255,255,255));
        if(is_slot1 == true)
        {
            slot1DC.setFillColor(Color(222, 55, 55));
            slot1DCText.setString("Delete");
        }
        else if(is_slot1 != true)
        {
            slot1DC.setFillColor(Color(96,184,84));
            slot1DCText.setString("Create");
        }
        FloatRect slot1DCTextRect = slot1DCText.getLocalBounds();
        slot1DCText.setOrigin(slot1DCTextRect.left+slot1DCTextRect.width/2.0f,slot1DCTextRect.top+slot1DCTextRect.height/2.0f);
        slot1DCText.setPosition(slot1DC.getPosition().x+slot1DC.getSize().x/2, slot1DC.getPosition().y+slot1DC.getSize().y/2);


        //Slot2:
        RectangleShape slot2;
        slot2.setSize(Vector2f(VideoMode::getDesktopMode().width/4, VideoMode::getDesktopMode().height / 10));
        slot2.setPosition(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height/10 + slot2.getSize().y*1.3*1));
        slot2.setOutlineColor(Color(255,255,255));
        slot2.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot2Text;
        slot2Text.setFont(font);
        slot2Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot2Text.setFillColor(Color(255,255,255));
        if(is_slot2 == true)
        {
            slot2.setFillColor(Color(96, 184, 84));
            slot2Text.setString("Save slot #2");
        }
        else if(is_slot2 != true)
        {
            slot2.setFillColor(Color(113, 127, 128));
            slot2Text.setString("Empty");
        }
        FloatRect slot2TextRect = slot2Text.getLocalBounds();
        slot2Text.setOrigin(slot2TextRect.left+slot2TextRect.width/2.0f,slot2TextRect.top+slot2TextRect.height/2.0f);
        slot2Text.setPosition(slot2.getPosition().x+slot2.getSize().x/2, slot2.getPosition().y+slot2.getSize().y/2);

        //Slot2 delete/crate
        RectangleShape slot2DC;
        slot2DC.setSize(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height / 10));
        slot2DC.setPosition(Vector2f(VideoMode::getDesktopMode().width/10 + slot2.getSize().x*1.1, VideoMode::getDesktopMode().height/10 + slot2.getSize().y*1.3*1));
        slot2DC.setOutlineColor(Color(255,255,255));
        slot2DC.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot2DCText;
        slot2DCText.setFont(font);
        slot2DCText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot2DCText.setFillColor(Color(255,255,255));
        if(is_slot2 == true)
        {
            slot2DC.setFillColor(Color(222, 55, 55));
            slot2DCText.setString("Delete");
        }
        else if(is_slot2 != true)
        {
            slot2DC.setFillColor(Color(96,184,84));
            slot2DCText.setString("Create");
        }
        FloatRect slot2DCTextRect = slot2DCText.getLocalBounds();
        slot2DCText.setOrigin(slot2DCTextRect.left+slot2DCTextRect.width/2.0f,slot2DCTextRect.top+slot2DCTextRect.height/2.0f);
        slot2DCText.setPosition(slot2DC.getPosition().x+slot2DC.getSize().x/2, slot2DC.getPosition().y+slot2DC.getSize().y/2);

        //Slot3:
        RectangleShape slot3;
        slot3.setSize(Vector2f(VideoMode::getDesktopMode().width/4, VideoMode::getDesktopMode().height / 10));
        slot3.setPosition(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height/10 + slot3.getSize().y*1.3*2));
        slot3.setOutlineColor(Color(255,255,255));
        slot3.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot3Text;
        slot3Text.setFont(font);
        slot3Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot3Text.setFillColor(Color(255,255,255));
        if(is_slot3 == true)
        {
            slot3.setFillColor(Color(96, 184, 84));
            slot3Text.setString("Save slot #3");
        }
        else if(is_slot3 != true)
        {
            slot3.setFillColor(Color(113, 127, 128));
            slot3Text.setString("Empty");
        }
        FloatRect slot3TextRect = slot3Text.getLocalBounds();
        slot3Text.setOrigin(slot3TextRect.left+slot3TextRect.width/2.0f,slot3TextRect.top+slot3TextRect.height/2.0f);
        slot3Text.setPosition(slot3.getPosition().x+slot3.getSize().x/2, slot3.getPosition().y+slot3.getSize().y/2);

        //Slot3 delete/crate
        RectangleShape slot3DC;
        slot3DC.setSize(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height / 10));
        slot3DC.setPosition(Vector2f(VideoMode::getDesktopMode().width/10 + slot3.getSize().x*1.1, VideoMode::getDesktopMode().height/10 + slot3.getSize().y*1.3*2));
        slot3DC.setOutlineColor(Color(255,255,255));
        slot3DC.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot3DCText;
        slot3DCText.setFont(font);
        slot3DCText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot3DCText.setFillColor(Color(255,255,255));
        if(is_slot3 == true)
        {
            slot3DC.setFillColor(Color(222, 55, 55));
            slot3DCText.setString("Delete");
        }
        else if(is_slot3 != true)
        {
            slot3DC.setFillColor(Color(96,184,84));
            slot3DCText.setString("Create");
        }
        FloatRect slot3DCTextRect = slot3DCText.getLocalBounds();
        slot3DCText.setOrigin(slot3DCTextRect.left+slot3DCTextRect.width/2.0f,slot3DCTextRect.top+slot3DCTextRect.height/2.0f);
        slot3DCText.setPosition(slot3DC.getPosition().x+slot3DC.getSize().x/2, slot3DC.getPosition().y+slot3DC.getSize().y/2);

        //Slot4:
        RectangleShape slot4;
        slot4.setSize(Vector2f(VideoMode::getDesktopMode().width/4, VideoMode::getDesktopMode().height / 10));
        slot4.setPosition(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height/10 + slot4.getSize().y*1.3*3));
        slot4.setOutlineColor(Color(255,255,255));
        slot4.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot4Text;
        slot4Text.setFont(font);
        slot4Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot4Text.setFillColor(Color(255,255,255));
        if(is_slot4 == true)
        {
            slot4.setFillColor(Color(96, 184, 84));
            slot4Text.setString("Save slot #4");
        }
        else if(is_slot4 != true)
        {
            slot4.setFillColor(Color(113, 127, 128));
            slot4Text.setString("Empty");
        }
        FloatRect slot4TextRect = slot4Text.getLocalBounds();
        slot4Text.setOrigin(slot4TextRect.left+slot4TextRect.width/2.0f,slot4TextRect.top+slot4TextRect.height/2.0f);
        slot4Text.setPosition(slot4.getPosition().x+slot4.getSize().x/2, slot4.getPosition().y+slot4.getSize().y/2);

        //Slot4 delete/crate
        RectangleShape slot4DC;
        slot4DC.setSize(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height / 10));
        slot4DC.setPosition(Vector2f(VideoMode::getDesktopMode().width/10 + slot4.getSize().x*1.1, VideoMode::getDesktopMode().height/10 + slot4.getSize().y*1.3*3));
        slot4DC.setOutlineColor(Color(255,255,255));
        slot4DC.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text slot4DCText;
        slot4DCText.setFont(font);
        slot4DCText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        slot4DCText.setFillColor(Color(255,255,255));
        if(is_slot4 == true)
        {
            slot4DC.setFillColor(Color(222, 55, 55));
            slot4DCText.setString("Delete");
        }
        else if(is_slot4 != true)
        {
            slot4DC.setFillColor(Color(96,184,84));
            slot4DCText.setString("Create");
        }
        FloatRect slot4DCTextRect = slot4DCText.getLocalBounds();
        slot4DCText.setOrigin(slot4DCTextRect.left+slot4DCTextRect.width/2.0f,slot4DCTextRect.top+slot4DCTextRect.height/2.0f);
        slot4DCText.setPosition(slot4DC.getPosition().x+slot4DC.getSize().x/2, slot4DC.getPosition().y+slot4DC.getSize().y/2);

        //LoadingScreen
        Text loadingText;
        loadingText.setFont(font);
        loadingText.setString("Loading...");
        loadingText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        loadingText.setFillColor(Color(255,255,255));
        loadingText.setOutlineColor(Color(0,0,0));
        loadingText.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        FloatRect loadingTextRect = loadingText.getLocalBounds();
        loadingText.setOrigin(loadingTextRect.left+loadingTextRect.width/2.0f,loadingTextRect.top+loadingTextRect.height/2.0f);
        loadingText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));

        //DeleteConfirmationMessage:
        RectangleShape deleteMsg;
        deleteMsg.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        deleteMsg.setOrigin(Vector2f(deleteMsg.getLocalBounds().width/2, deleteMsg.getGlobalBounds().height/2));
        deleteMsg.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));
        deleteMsg.setFillColor(Color(1,14,36,0));
        deleteMsg.setOutlineColor(Color(1,14,36,0));
        deleteMsg.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text deleteMsgText;
        deleteMsgText.setFont(font);
        deleteMsgText.setString("Are you sure you want to delete this save?");
        deleteMsgText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        deleteMsgText.setFillColor(Color(255,255,255));
        FloatRect deleteMsgTextRect = deleteMsgText.getLocalBounds();
        deleteMsgText.setOrigin(deleteMsgTextRect.left+deleteMsgTextRect.width/2.0f,deleteMsgTextRect.top+deleteMsgTextRect.height/2.0f);
        deleteMsgText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));
        deleteMsgText.setOutlineColor(Color(0,0,0));
        deleteMsgText.setOutlineThickness((VideoMode::getDesktopMode().width*8)/1920);


        //DeleteConfirmationMessageYES:
        RectangleShape deleteMsgYes;
        deleteMsgYes.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        deleteMsgYes.setOrigin(Vector2f(deleteMsgYes.getLocalBounds().width/2, deleteMsgYes.getGlobalBounds().height/2));
        deleteMsgYes.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 - deleteMsgYes.getSize().x, VideoMode::getDesktopMode().height/2 + deleteMsgYes.getSize().y*1.2));
        deleteMsgYes.setFillColor(Color(222, 55, 55));
        deleteMsgYes.setOutlineColor(Color(255,255,255));
        deleteMsgYes.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text deleteMsgYesText;
        deleteMsgYesText.setFont(font);
        deleteMsgYesText.setString("Delete");
        deleteMsgYesText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        deleteMsgYesText.setFillColor(Color(255,255,255));
        FloatRect deleteMsgYesTextRect = deleteMsgYesText.getLocalBounds();
        deleteMsgYesText.setOrigin(deleteMsgYesTextRect.left+deleteMsgYesTextRect.width/2.0f,deleteMsgYesTextRect.top+deleteMsgYesTextRect.height/2.0f);
        deleteMsgYesText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 - deleteMsgYes.getSize().x, VideoMode::getDesktopMode().height/2 + deleteMsgYes.getSize().y*1.2));

        //DeleteConfirmationMessageNo:
        RectangleShape deleteMsgNo;
        deleteMsgNo.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        deleteMsgNo.setOrigin(Vector2f(deleteMsgNo.getLocalBounds().width/2, deleteMsgNo.getGlobalBounds().height/2));
        deleteMsgNo.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + deleteMsgNo.getSize().x, VideoMode::getDesktopMode().height/2 + deleteMsgNo.getSize().y*1.2));
        deleteMsgNo.setFillColor(Color(96, 184, 84));
        deleteMsgNo.setOutlineColor(Color(255,255,255));
        deleteMsgNo.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text deleteMsgNoText;
        deleteMsgNoText.setFont(font);
        deleteMsgNoText.setString("Cancel");
        deleteMsgNoText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        deleteMsgNoText.setFillColor(Color(255,255,255));
        FloatRect deleteMsgNoTextRect = deleteMsgNoText.getLocalBounds();
        deleteMsgNoText.setOrigin(deleteMsgNoTextRect.left+deleteMsgNoTextRect.width/2.0f,deleteMsgNoTextRect.top+deleteMsgNoTextRect.height/2.0f);
        deleteMsgNoText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + deleteMsgNo.getSize().x, VideoMode::getDesktopMode().height/2 + deleteMsgNo.getSize().y*1.2));

        //Guide Button:
        RectangleShape guideButton;
        guideButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        guideButton.setOrigin(Vector2f(guideButton.getLocalBounds().width/2, guideButton.getGlobalBounds().height/2));
        guideButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2 + guideButton.getSize().y*1.2));
        guideButton.setFillColor(Color(20, 206, 219));
        guideButton.setOutlineColor(Color(255,255,255));
        guideButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text guideButtonText;
        guideButtonText.setFont(font);
        guideButtonText.setString("Guide");
        guideButtonText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        guideButtonText.setFillColor(Color(255,255,255));
        FloatRect guideButtonTextRect = guideButtonText.getLocalBounds();
        guideButtonText.setOrigin(guideButtonTextRect.left+guideButtonTextRect.width/2.0f,guideButtonTextRect.top+guideButtonTextRect.height/2.0f);
        guideButtonText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2 + guideButton.getSize().y*1.2));

        //GuideBackButton:
        RectangleShape guideBackButton;
        guideBackButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        guideBackButton.setOrigin(Vector2f(guideBackButton.getLocalBounds().width/2, guideBackButton.getGlobalBounds().height/2));
        guideBackButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + guideBackButton.getSize().x*1.2*2, VideoMode::getDesktopMode().height/2 + guideBackButton.getSize().y*1.2*3));
        guideBackButton.setFillColor(Color(222, 55, 55));
        guideBackButton.setOutlineColor(Color(255,255,255));
        guideBackButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text guideBackText;
        guideBackText.setFont(font);
        guideBackText.setString("Back");
        guideBackText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        guideBackText.setFillColor(Color(255,255,255));
        FloatRect guideBackTextRect = guideBackText.getLocalBounds();
        guideBackText.setOrigin(guideBackTextRect.left+guideBackTextRect.width/2.0f,guideBackTextRect.top+guideBackTextRect.height/2.0f);
        guideBackText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + guideBackButton.getSize().x*1.2*2, VideoMode::getDesktopMode().height/2 + guideBackButton.getSize().y*1.2*3));

        //Guide Message:
        Text guideMessageText;
        guideMessageText.setFont(font);
        guideMessageText.setString("Press 'Escape' to open the PAUSE menu.\nPress 'F11' to toggle Fullscreen.\nPress 'I' to open the inventory.\nPress 'W'/'A'/'S'/'D' to move the camera around.\nPress 'Ctrl' + '+'/'-' to change zoom level.\nPress 'Q' to pick a block from the ground.\nPress 'R' to rotate the block in hand.\nPress 'Left Mouse Click' to place blocks\n\nPlacing an empty block on the ground will remove the already existing block.\nRefer to the guide from the game folder for more information regarding each block.");
        guideMessageText.setCharacterSize((VideoMode::getDesktopMode().width*40)/1920);
        guideMessageText.setFillColor(Color(255,255,255));
        guideMessageText.setPosition(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height/10));
        guideMessageText.setOutlineColor(Color(0,0,0));
        guideMessageText.setOutlineThickness((VideoMode::getDesktopMode().width*8)/1920);

        //Options Button:
        RectangleShape optionsButton;
        optionsButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        optionsButton.setOrigin(Vector2f(optionsButton.getLocalBounds().width/2, optionsButton.getGlobalBounds().height/2));
        optionsButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2 + optionsButton.getSize().y*1.2*2));
        optionsButton.setFillColor(Color(113, 127, 128));
        optionsButton.setOutlineColor(Color(255,255,255));
        optionsButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text optionsButtonText;
        optionsButtonText.setFont(font);
        optionsButtonText.setString("Options");
        optionsButtonText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        optionsButtonText.setFillColor(Color(255,255,255));
        FloatRect optionsButtonTextRect = optionsButtonText.getLocalBounds();
        optionsButtonText.setOrigin(optionsButtonTextRect.left+optionsButtonTextRect.width/2.0f,optionsButtonTextRect.top+optionsButtonTextRect.height/2.0f);
        optionsButtonText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2 + optionsButton.getSize().y*1.2*2));

        //OptionsBackButton:
        RectangleShape optionsBackButton;
        optionsBackButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        optionsBackButton.setOrigin(Vector2f(optionsBackButton.getLocalBounds().width/2, optionsBackButton.getGlobalBounds().height/2));
        optionsBackButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + optionsBackButton.getSize().x*1.2*2, VideoMode::getDesktopMode().height/2 + optionsBackButton.getSize().y*1.2*3));
        optionsBackButton.setFillColor(Color(222, 55, 55));
        optionsBackButton.setOutlineColor(Color(255,255,255));
        optionsBackButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text optionsBackText;
        optionsBackText.setFont(font);
        optionsBackText.setString("Back");
        optionsBackText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        optionsBackText.setFillColor(Color(255,255,255));
        FloatRect optionsBackTextRect = optionsBackText.getLocalBounds();
        optionsBackText.setOrigin(optionsBackTextRect.left+optionsBackTextRect.width/2.0f,optionsBackTextRect.top+optionsBackTextRect.height/2.0f);
        optionsBackText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + optionsBackButton.getSize().x*1.2*2, VideoMode::getDesktopMode().height/2 + optionsBackButton.getSize().y*1.2*3));

        //OptionsApplyButton:
        RectangleShape optionsApplyButton;
        optionsApplyButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        optionsApplyButton.setOrigin(Vector2f(optionsApplyButton.getLocalBounds().width/2, optionsApplyButton.getGlobalBounds().height/2));
        optionsApplyButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + optionsApplyButton.getSize().x*1.2*1, VideoMode::getDesktopMode().height/2 + optionsApplyButton.getSize().y*1.2*3));
        optionsApplyButton.setFillColor(Color(96, 184, 84));
        optionsApplyButton.setOutlineColor(Color(255,255,255));
        optionsApplyButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text optionsApplyText;
        optionsApplyText.setFont(font);
        optionsApplyText.setString("Apply");
        optionsApplyText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        optionsApplyText.setFillColor(Color(255,255,255));
        FloatRect optionsApplyTextRect = optionsApplyText.getLocalBounds();
        optionsApplyText.setOrigin(optionsApplyTextRect.left+optionsApplyTextRect.width/2.0f,optionsApplyTextRect.top+optionsApplyTextRect.height/2.0f);
        optionsApplyText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + optionsApplyButton.getSize().x*1.2*1, VideoMode::getDesktopMode().height/2 + optionsApplyButton.getSize().y*1.2*3));

        //OptionsMessage:
        Text optionsMessage;
        optionsMessage.setFont(font);
        optionsMessage.setString("Fullscreen           :\nResolution           :\nMusic volume         :\nSFX volume           :\nLevel update delay   :\nTexturePack          :");
        optionsMessage.setCharacterSize((VideoMode::getDesktopMode().width*40)/1920);
        optionsMessage.setFillColor(Color(255,255,255));
        optionsMessage.setPosition(Vector2f(VideoMode::getDesktopMode().width/10, VideoMode::getDesktopMode().height/10));
        optionsMessage.setOutlineColor(Color(0,0,0));
        optionsMessage.setOutlineThickness((VideoMode::getDesktopMode().width*8)/1920);
        optionsMessage.setLineSpacing(3);

        //OptionFullscreenOn:
        RectangleShape opFullscreenOn;
        opFullscreenOn.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        opFullscreenOn.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5, VideoMode::getDesktopMode().height/10));
        if(tempFullscreen == true)
            opFullscreenOn.setFillColor(Color(96, 184, 84));
        else if(tempFullscreen == false)
            opFullscreenOn.setFillColor(Color(113, 127, 128));
        opFullscreenOn.setOutlineColor(Color(255,255,255));
        opFullscreenOn.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text opFullscreenOnText;
        opFullscreenOnText.setFont(font);
        opFullscreenOnText.setString("On");
        opFullscreenOnText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        opFullscreenOnText.setFillColor(Color(255,255,255));
        FloatRect opFullScreenOnTextRect = opFullscreenOnText.getLocalBounds();
        opFullscreenOnText.setOrigin(opFullScreenOnTextRect.left+opFullScreenOnTextRect.width/2.0f,opFullScreenOnTextRect.top+opFullScreenOnTextRect.height/2.0f);
        opFullscreenOnText.setPosition(opFullscreenOn.getPosition().x+opFullscreenOn.getSize().x/2, opFullscreenOn.getPosition().y+opFullscreenOn.getSize().y/2);

        //Option FullscreenOFF:
        RectangleShape opFullscreenOff;
        opFullscreenOff.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        opFullscreenOff.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + opFullscreenOff.getSize().x*1.2, VideoMode::getDesktopMode().height/10));
        if(tempFullscreen == false)
            opFullscreenOff.setFillColor(Color(96, 184, 84));
        else if(tempFullscreen == true)
            opFullscreenOff.setFillColor(Color(113, 127, 128));
        opFullscreenOff.setOutlineColor(Color(255,255,255));
        opFullscreenOff.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text opFullscreenOffText;
        opFullscreenOffText.setFont(font);
        opFullscreenOffText.setString("Off");
        opFullscreenOffText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        opFullscreenOffText.setFillColor(Color(255,255,255));
        FloatRect opFullscreenOffTextRect = opFullscreenOffText.getLocalBounds();
        opFullscreenOffText.setOrigin(opFullscreenOffTextRect.left+opFullscreenOffTextRect.width/2.0f,opFullscreenOffTextRect.top+opFullscreenOffTextRect.height/2.0f);
        opFullscreenOffText.setPosition(opFullscreenOff.getPosition().x+opFullscreenOff.getSize().x/2, opFullscreenOff.getPosition().y+opFullscreenOff.getSize().y/2);

        //Options res1600x900
        RectangleShape res1600x900;
        res1600x900.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        res1600x900.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + res1600x900.getSize().x*1.2*0, VideoMode::getDesktopMode().height/10 + res1600x900.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 1600 && tempWindowModeSizeY == 900)
            res1600x900.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 1600 && tempWindowModeSizeY != 900)
            res1600x900.setFillColor(Color(113, 127, 128));
        res1600x900.setOutlineColor(Color(255,255,255));
        res1600x900.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text res1600x900Text;
        res1600x900Text.setFont(font);
        res1600x900Text.setString("1600x900");
        res1600x900Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        res1600x900Text.setFillColor(Color(255,255,255));
        FloatRect res1600x900TextRect = res1600x900Text.getLocalBounds();
        res1600x900Text.setOrigin(res1600x900TextRect.left+res1600x900TextRect.width/2.0f,res1600x900TextRect.top+res1600x900TextRect.height/2.0f);
        res1600x900Text.setPosition(res1600x900.getPosition().x+res1600x900.getSize().x/2, res1600x900.getPosition().y+res1600x900.getSize().y/2);

        //Options res1366x768
        RectangleShape res1366x768;
        res1366x768.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        res1366x768.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + res1366x768.getSize().x*1.2*1, VideoMode::getDesktopMode().height/10 + res1366x768.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 1366 && tempWindowModeSizeY == 768)
            res1366x768.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 1366 && tempWindowModeSizeY != 768)
            res1366x768.setFillColor(Color(113, 127, 128));
        res1366x768.setOutlineColor(Color(255,255,255));
        res1366x768.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text res1366x768Text;
        res1366x768Text.setFont(font);
        res1366x768Text.setString("1366x768");
        res1366x768Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        res1366x768Text.setFillColor(Color(255,255,255));
        FloatRect res1366x768TextRect = res1366x768Text.getLocalBounds();
        res1366x768Text.setOrigin(res1366x768TextRect.left+res1366x768TextRect.width/2.0f,res1366x768TextRect.top+res1366x768TextRect.height/2.0f);
        res1366x768Text.setPosition(res1366x768.getPosition().x+res1366x768.getSize().x/2, res1366x768.getPosition().y+res1366x768.getSize().y/2);

        //Options res1024x576
        RectangleShape res1024x576;
        res1024x576.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        res1024x576.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + res1024x576.getSize().x*1.2*2, VideoMode::getDesktopMode().height/10 + res1024x576.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 1024 && tempWindowModeSizeY == 576)
            res1024x576.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 1024 && tempWindowModeSizeY != 576)
            res1024x576.setFillColor(Color(113, 127, 128));
        res1024x576.setOutlineColor(Color(255,255,255));
        res1024x576.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text res1024x576Text;
        res1024x576Text.setFont(font);
        res1024x576Text.setString("1024x576");
        res1024x576Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        res1024x576Text.setFillColor(Color(255,255,255));
        FloatRect res1024x576TextRect = res1024x576Text.getLocalBounds();
        res1024x576Text.setOrigin(res1024x576TextRect.left+res1024x576TextRect.width/2.0f,res1024x576TextRect.top+res1024x576TextRect.height/2.0f);
        res1024x576Text.setPosition(res1024x576.getPosition().x+res1024x576.getSize().x/2, res1024x576.getPosition().y+res1024x576.getSize().y/2);

        //Options res800x450
        RectangleShape res800x450;
        res800x450.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        res800x450.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + res800x450.getSize().x*1.2*3, VideoMode::getDesktopMode().height/10 + res800x450.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 800 && tempWindowModeSizeY == 450)
            res800x450.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 800 && tempWindowModeSizeY != 450)
            res800x450.setFillColor(Color(113, 127, 128));
        res800x450.setOutlineColor(Color(255,255,255));
        res800x450.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text res800x450Text;
        res800x450Text.setFont(font);
        res800x450Text.setString("800x450");
        res800x450Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        res800x450Text.setFillColor(Color(255,255,255));
        FloatRect res800x450TextRect = res800x450Text.getLocalBounds();
        res800x450Text.setOrigin(res800x450TextRect.left+res800x450TextRect.width/2.0f,res800x450TextRect.top+res800x450TextRect.height/2.0f);
        res800x450Text.setPosition(res800x450.getPosition().x+res800x450.getSize().x/2, res800x450.getPosition().y+res800x450.getSize().y/2);

        //Options music 0%
        RectangleShape musicVol0;
        musicVol0.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        musicVol0.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + musicVol0.getSize().x*1.2*0, VideoMode::getDesktopMode().height/10 + musicVol0.getSize().y*1.1*4));
        if(musicVolume == 0)
            musicVol0.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 0)
            musicVol0.setFillColor(Color(113, 127, 128));
        musicVol0.setOutlineColor(Color(255,255,255));
        musicVol0.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text musicVol0Text;
        musicVol0Text.setFont(font);
        musicVol0Text.setString("0%");
        musicVol0Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        musicVol0Text.setFillColor(Color(255,255,255));
        FloatRect musicVol0TextRect = musicVol0Text.getLocalBounds();
        musicVol0Text.setOrigin(musicVol0TextRect.left+musicVol0TextRect.width/2.0f,musicVol0TextRect.top+musicVol0TextRect.height/2.0f);
        musicVol0Text.setPosition(musicVol0.getPosition().x+musicVol0.getSize().x/2, musicVol0.getPosition().y+musicVol0.getSize().y/2);

        //Options music 25%
        RectangleShape musicVol25;
        musicVol25.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        musicVol25.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + musicVol25.getSize().x*1.2*1, VideoMode::getDesktopMode().height/10 + musicVol25.getSize().y*1.1*4));
        if(musicVolume == 0.25)
            musicVol25.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 0.25)
            musicVol25.setFillColor(Color(113, 127, 128));
        musicVol25.setOutlineColor(Color(255,255,255));
        musicVol25.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text musicVol25Text;
        musicVol25Text.setFont(font);
        musicVol25Text.setString("25%");
        musicVol25Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        musicVol25Text.setFillColor(Color(255,255,255));
        FloatRect musicVol25TextRect = musicVol25Text.getLocalBounds();
        musicVol25Text.setOrigin(musicVol25TextRect.left+musicVol25TextRect.width/2.0f,musicVol25TextRect.top+musicVol25TextRect.height/2.0f);
        musicVol25Text.setPosition(musicVol25.getPosition().x+musicVol25.getSize().x/2, musicVol25.getPosition().y+musicVol25.getSize().y/2);

        //Options music 50%
        RectangleShape musicVol50;
        musicVol50.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        musicVol50.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + musicVol50.getSize().x*1.2*2, VideoMode::getDesktopMode().height/10 + musicVol50.getSize().y*1.1*4));
        if(musicVolume == 0.50)
            musicVol50.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 0.50)
            musicVol50.setFillColor(Color(113, 127, 128));
        musicVol50.setOutlineColor(Color(255,255,255));
        musicVol50.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text musicVol50Text;
        musicVol50Text.setFont(font);
        musicVol50Text.setString("50%");
        musicVol50Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        musicVol50Text.setFillColor(Color(255,255,255));
        FloatRect musicVol50TextRect = musicVol50Text.getLocalBounds();
        musicVol50Text.setOrigin(musicVol50TextRect.left+musicVol50TextRect.width/2.0f,musicVol50TextRect.top+musicVol50TextRect.height/2.0f);
        musicVol50Text.setPosition(musicVol50.getPosition().x+musicVol50.getSize().x/2, musicVol50.getPosition().y+musicVol50.getSize().y/2);

        //Options music 100%
        RectangleShape musicVol100;
        musicVol100.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        musicVol100.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + musicVol100.getSize().x*1.2*3, VideoMode::getDesktopMode().height/10 + musicVol100.getSize().y*1.1*4));
        if(musicVolume == 1)
            musicVol100.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 1)
            musicVol100.setFillColor(Color(113, 127, 128));
        musicVol100.setOutlineColor(Color(255,255,255));
        musicVol100.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text musicVol100Text;
        musicVol100Text.setFont(font);
        musicVol100Text.setString("100%");
        musicVol100Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        musicVol100Text.setFillColor(Color(255,255,255));
        FloatRect musicVol100TextRect = musicVol100Text.getLocalBounds();
        musicVol100Text.setOrigin(musicVol100TextRect.left+musicVol100TextRect.width/2.0f,musicVol100TextRect.top+musicVol100TextRect.height/2.0f);
        musicVol100Text.setPosition(musicVol100.getPosition().x+musicVol100.getSize().x/2, musicVol100.getPosition().y+musicVol100.getSize().y/2);

        //Options sfx 0%
        RectangleShape sfxVol0;
        sfxVol0.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        sfxVol0.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + sfxVol0.getSize().x*1.2*0, VideoMode::getDesktopMode().height/10 + sfxVol0.getSize().y*1.1*6));
        if(sfxVolume == 0)
            sfxVol0.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 0)
            sfxVol0.setFillColor(Color(113, 127, 128));
        sfxVol0.setOutlineColor(Color(255,255,255));
        sfxVol0.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text sfxVol0Text;
        sfxVol0Text.setFont(font);
        sfxVol0Text.setString("0%");
        sfxVol0Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        sfxVol0Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol0TextRect = sfxVol0Text.getLocalBounds();
        sfxVol0Text.setOrigin(sfxVol0TextRect.left+sfxVol0TextRect.width/2.0f,sfxVol0TextRect.top+sfxVol0TextRect.height/2.0f);
        sfxVol0Text.setPosition(sfxVol0.getPosition().x+sfxVol0.getSize().x/2, sfxVol0.getPosition().y+sfxVol0.getSize().y/2);

        //Options sfx 25%
        RectangleShape sfxVol25;
        sfxVol25.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        sfxVol25.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + sfxVol25.getSize().x*1.2*1, VideoMode::getDesktopMode().height/10 + sfxVol25.getSize().y*1.1*6));
        if(sfxVolume == 0.25)
            sfxVol25.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 0.25)
            sfxVol25.setFillColor(Color(113, 127, 128));
        sfxVol25.setOutlineColor(Color(255,255,255));
        sfxVol25.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text sfxVol25Text;
        sfxVol25Text.setFont(font);
        sfxVol25Text.setString("25%");
        sfxVol25Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        sfxVol25Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol25TextRect = sfxVol25Text.getLocalBounds();
        sfxVol25Text.setOrigin(sfxVol25TextRect.left+sfxVol25TextRect.width/2.0f,sfxVol25TextRect.top+sfxVol25TextRect.height/2.0f);
        sfxVol25Text.setPosition(sfxVol25.getPosition().x+sfxVol25.getSize().x/2, sfxVol25.getPosition().y+sfxVol25.getSize().y/2);

        //Options sfx 50%
        RectangleShape sfxVol50;
        sfxVol50.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        sfxVol50.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + sfxVol50.getSize().x*1.2*2, VideoMode::getDesktopMode().height/10 + sfxVol50.getSize().y*1.1*6));
        if(sfxVolume == 0.50)
            sfxVol50.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 0.50)
            sfxVol50.setFillColor(Color(113, 127, 128));
        sfxVol50.setOutlineColor(Color(255,255,255));
        sfxVol50.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text sfxVol50Text;
        sfxVol50Text.setFont(font);
        sfxVol50Text.setString("50%");
        sfxVol50Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        sfxVol50Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol50TextRect = sfxVol50Text.getLocalBounds();
        sfxVol50Text.setOrigin(sfxVol50TextRect.left+sfxVol50TextRect.width/2.0f,sfxVol50TextRect.top+sfxVol50TextRect.height/2.0f);
        sfxVol50Text.setPosition(sfxVol50.getPosition().x+sfxVol50.getSize().x/2, sfxVol50.getPosition().y+sfxVol50.getSize().y/2);

        //Options sfx 100%
        RectangleShape sfxVol100;
        sfxVol100.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        sfxVol100.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + sfxVol100.getSize().x*1.2*3, VideoMode::getDesktopMode().height/10 + sfxVol100.getSize().y*1.1*6));
        if(sfxVolume == 1)
            sfxVol100.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 1)
            sfxVol100.setFillColor(Color(113, 127, 128));
        sfxVol100.setOutlineColor(Color(255,255,255));
        sfxVol100.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text sfxVol100Text;
        sfxVol100Text.setFont(font);
        sfxVol100Text.setString("100%");
        sfxVol100Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        sfxVol100Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol100TextRect = sfxVol100Text.getLocalBounds();
        sfxVol100Text.setOrigin(sfxVol100TextRect.left+sfxVol100TextRect.width/2.0f,sfxVol100TextRect.top+sfxVol100TextRect.height/2.0f);
        sfxVol100Text.setPosition(sfxVol100.getPosition().x+sfxVol100.getSize().x/2, sfxVol100.getPosition().y+sfxVol100.getSize().y/2);

        //Options Delay 0.1
        RectangleShape delay01;
        delay01.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        delay01.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + delay01.getSize().x*1.2*0, VideoMode::getDesktopMode().height/10 + delay01.getSize().y*1.1*8));
        if(levelUpdateDelay == (float)0.1)
            delay01.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 0.1)
            delay01.setFillColor(Color(113, 127, 128));
        delay01.setOutlineColor(Color(255,255,255));
        delay01.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text delay01Text;
        delay01Text.setFont(font);
        delay01Text.setString("0.1s");
        delay01Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        delay01Text.setFillColor(Color(255,255,255));
        FloatRect delay01TextRect = delay01Text.getLocalBounds();
        delay01Text.setOrigin(delay01TextRect.left+delay01TextRect.width/2.0f,delay01TextRect.top+delay01TextRect.height/2.0f);
        delay01Text.setPosition(delay01.getPosition().x+delay01.getSize().x/2, delay01.getPosition().y+delay01.getSize().y/2);

        //Options Delay 0.5
        RectangleShape delay05;
        delay05.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        delay05.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + delay05.getSize().x*1.2*1, VideoMode::getDesktopMode().height/10 + delay05.getSize().y*1.1*8));
        if(levelUpdateDelay == 0.5)
            delay05.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 0.5)
            delay05.setFillColor(Color(113, 127, 128));
        delay05.setOutlineColor(Color(255,255,255));
        delay05.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text delay05Text;
        delay05Text.setFont(font);
        delay05Text.setString("0.5s");
        delay05Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        delay05Text.setFillColor(Color(255,255,255));
        FloatRect delay05TextRect = delay05Text.getLocalBounds();
        delay05Text.setOrigin(delay05TextRect.left+delay05TextRect.width/2.0f,delay05TextRect.top+delay05TextRect.height/2.0f);
        delay05Text.setPosition(delay05.getPosition().x+delay05.getSize().x/2, delay05.getPosition().y+delay05.getSize().y/2);

        //Options Delay 10
        RectangleShape delay10;
        delay10.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        delay10.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + delay10.getSize().x*1.2*2, VideoMode::getDesktopMode().height/10 + delay10.getSize().y*1.1*8));
        if(levelUpdateDelay == 1)
            delay10.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 1)
            delay10.setFillColor(Color(113, 127, 128));
        delay10.setOutlineColor(Color(255,255,255));
        delay10.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text delay10Text;
        delay10Text.setFont(font);
        delay10Text.setString("1s");
        delay10Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        delay10Text.setFillColor(Color(255,255,255));
        FloatRect delay10TextRect = delay10Text.getLocalBounds();
        delay10Text.setOrigin(delay10TextRect.left+delay10TextRect.width/2.0f,delay10TextRect.top+delay10TextRect.height/2.0f);
        delay10Text.setPosition(delay10.getPosition().x+delay10.getSize().x/2, delay10.getPosition().y+delay10.getSize().y/2);

        //Options Delay 20
        RectangleShape delay20;
        delay20.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        delay20.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + delay20.getSize().x*1.2*3, VideoMode::getDesktopMode().height/10 + delay20.getSize().y*1.1*8));
        if(levelUpdateDelay == 2)
            delay20.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 2)
            delay20.setFillColor(Color(113, 127, 128));
        delay20.setOutlineColor(Color(255,255,255));
        delay20.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text delay20Text;
        delay20Text.setFont(font);
        delay20Text.setString("2s");
        delay20Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        delay20Text.setFillColor(Color(255,255,255));
        FloatRect delay20TextRect = delay20Text.getLocalBounds();
        delay20Text.setOrigin(delay20TextRect.left+delay20TextRect.width/2.0f,delay20TextRect.top+delay20TextRect.height/2.0f);
        delay20Text.setPosition(delay20.getPosition().x+delay20.getSize().x/2, delay20.getPosition().y+delay20.getSize().y/2);

        //Options textureDefault
        RectangleShape txtDefault;
        txtDefault.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        txtDefault.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + txtDefault.getSize().x*1.2*0, VideoMode::getDesktopMode().height/10 + txtDefault.getSize().y*1.1*10));
        if(texturePath == "default")
            txtDefault.setFillColor(Color(96, 184, 84));
        else if(texturePath != "default")
            txtDefault.setFillColor(Color(113, 127, 128));
        txtDefault.setOutlineColor(Color(255,255,255));
        txtDefault.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text txtDefaultText;
        txtDefaultText.setFont(font);
        txtDefaultText.setString("Default");
        txtDefaultText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        txtDefaultText.setFillColor(Color(255,255,255));
        FloatRect txtDefaultTextRect = txtDefaultText.getLocalBounds();
        txtDefaultText.setOrigin(txtDefaultTextRect.left+txtDefaultTextRect.width/2.0f,txtDefaultTextRect.top+txtDefaultTextRect.height/2.0f);
        txtDefaultText.setPosition(txtDefault.getPosition().x+txtDefault.getSize().x/2, txtDefault.getPosition().y+txtDefault.getSize().y/2);

        //Options textureDark
        RectangleShape txtDark;
        txtDark.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        txtDark.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + txtDark.getSize().x*1.2*1, VideoMode::getDesktopMode().height/10 + txtDark.getSize().y*1.1*10));
        if(texturePath == "dark")
            txtDark.setFillColor(Color(96, 184, 84));
        else if(texturePath != "dark")
            txtDark.setFillColor(Color(113, 127, 128));
        txtDark.setOutlineColor(Color(255,255,255));
        txtDark.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text txtDarkText;
        txtDarkText.setFont(font);
        txtDarkText.setString("Dark");
        txtDarkText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        txtDarkText.setFillColor(Color(255,255,255));
        FloatRect txtDarkTextRect = txtDarkText.getLocalBounds();
        txtDarkText.setOrigin(txtDarkTextRect.left+txtDarkTextRect.width/2.0f,txtDarkTextRect.top+txtDarkTextRect.height/2.0f);
        txtDarkText.setPosition(txtDark.getPosition().x+txtDark.getSize().x/2, txtDark.getPosition().y+txtDark.getSize().y/2);

        //Options textureClassic
        RectangleShape txtClassic;
        txtClassic.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        txtClassic.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + txtClassic.getSize().x*1.2*2, VideoMode::getDesktopMode().height/10 + txtClassic.getSize().y*1.1*10));
        if(texturePath == "classic")
            txtClassic.setFillColor(Color(96, 184, 84));
        else if(texturePath != "classic")
            txtClassic.setFillColor(Color(113, 127, 128));
        txtClassic.setOutlineColor(Color(255,255,255));
        txtClassic.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text txtClassicText;
        txtClassicText.setFont(font);
        txtClassicText.setString("Classic");
        txtClassicText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        txtClassicText.setFillColor(Color(255,255,255));
        FloatRect txtClassicTextRect = txtClassicText.getLocalBounds();
        txtClassicText.setOrigin(txtClassicTextRect.left+txtClassicTextRect.width/2.0f,txtClassicTextRect.top+txtClassicTextRect.height/2.0f);
        txtClassicText.setPosition(txtClassic.getPosition().x+txtClassic.getSize().x/2, txtClassic.getPosition().y+txtClassic.getSize().y/2);

        //Options textureCustom
        RectangleShape txtCustom;
        txtCustom.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        txtCustom.setPosition(Vector2f(VideoMode::getDesktopMode().width/2.5 + txtCustom.getSize().x*1.2*3, VideoMode::getDesktopMode().height/10 + txtCustom.getSize().y*1.1*10));
        if(texturePath == "custom")
            txtCustom.setFillColor(Color(96, 184, 84));
        else if(texturePath != "custom")
            txtCustom.setFillColor(Color(113, 127, 128));
        txtCustom.setOutlineColor(Color(255,255,255));
        txtCustom.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text txtCustomText;
        txtCustomText.setFont(font);
        txtCustomText.setString("Custom");
        txtCustomText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        txtCustomText.setFillColor(Color(255,255,255));
        FloatRect txtCustomTextRect = txtCustomText.getLocalBounds();
        txtCustomText.setOrigin(txtCustomTextRect.left+txtCustomTextRect.width/2.0f,txtCustomTextRect.top+txtCustomTextRect.height/2.0f);
        txtCustomText.setPosition(txtCustom.getPosition().x+txtCustom.getSize().x/2, txtCustom.getPosition().y+txtCustom.getSize().y/2);


        //Exit Button:
        RectangleShape exitButton;
        exitButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        exitButton.setOrigin(Vector2f(exitButton.getLocalBounds().width/2, exitButton.getGlobalBounds().height/2));
        exitButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2 + exitButton.getSize().y*1.2*3));
        exitButton.setFillColor(Color(222, 55, 55));
        exitButton.setOutlineColor(Color(255,255,255));
        exitButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text exitButtonText;
        exitButtonText.setFont(font);
        exitButtonText.setString("Exit");
        exitButtonText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        exitButtonText.setFillColor(Color(255,255,255));
        FloatRect exitButtonTextRect = exitButtonText.getLocalBounds();
        exitButtonText.setOrigin(exitButtonTextRect.left+exitButtonTextRect.width/2.0f,exitButtonTextRect.top+exitButtonTextRect.height/2.0f);
        exitButtonText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2 + exitButton.getSize().y*1.2*3));

        //ExitConfirmationMessage:
        RectangleShape exitConfirmationMessage;
        exitConfirmationMessage.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        exitConfirmationMessage.setOrigin(Vector2f(exitConfirmationMessage.getLocalBounds().width/2, exitConfirmationMessage.getGlobalBounds().height/2));
        exitConfirmationMessage.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));
        exitConfirmationMessage.setFillColor(Color(1,14,36,0));
        exitConfirmationMessage.setOutlineColor(Color(1,14,36,0));
        exitConfirmationMessage.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text exitConfirmationText;
        exitConfirmationText.setFont(font);
        exitConfirmationText.setString("Are you sure you want to exit?");
        exitConfirmationText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        exitConfirmationText.setFillColor(Color(255,255,255));
        FloatRect exitConfirmationTextRect = exitConfirmationText.getLocalBounds();
        exitConfirmationText.setOrigin(exitConfirmationTextRect.left+exitConfirmationTextRect.width/2.0f,exitConfirmationTextRect.top+exitConfirmationTextRect.height/2.0f);
        exitConfirmationText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));
        exitConfirmationText.setOutlineColor(Color(0,0,0));
        exitConfirmationText.setOutlineThickness((VideoMode::getDesktopMode().width*8)/1920);


        //ExitConfirmationMessageYES:
        RectangleShape exitYesButton;
        exitYesButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        exitYesButton.setOrigin(Vector2f(exitYesButton.getLocalBounds().width/2, exitYesButton.getGlobalBounds().height/2));
        exitYesButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 - exitYesButton.getSize().x, VideoMode::getDesktopMode().height/2 + exitYesButton.getSize().y*1.2));
        exitYesButton.setFillColor(Color(222, 55, 55));
        exitYesButton.setOutlineColor(Color(255,255,255));
        exitYesButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text exitYesButtonText;
        exitYesButtonText.setFont(font);
        exitYesButtonText.setString("Exit");
        exitYesButtonText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        exitYesButtonText.setFillColor(Color(255,255,255));
        FloatRect exitYesButtonTextRect = exitYesButtonText.getLocalBounds();
        exitYesButtonText.setOrigin(exitYesButtonTextRect.left+exitYesButtonTextRect.width/2.0f,exitYesButtonTextRect.top+exitYesButtonTextRect.height/2.0f);
        exitYesButtonText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 - exitYesButton.getSize().x, VideoMode::getDesktopMode().height/2 + exitYesButton.getSize().y*1.2));

        //ExitConfirmationMessageNo:
        RectangleShape exitNoButton;
        exitNoButton.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        exitNoButton.setOrigin(Vector2f(exitNoButton.getLocalBounds().width/2, exitNoButton.getGlobalBounds().height/2));
        exitNoButton.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + exitNoButton.getSize().x, VideoMode::getDesktopMode().height/2 + exitNoButton.getSize().y*1.2));
        exitNoButton.setFillColor(Color(96, 184, 84));
        exitNoButton.setOutlineColor(Color(255,255,255));
        exitNoButton.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text exitNoButtonText;
        exitNoButtonText.setFont(font);
        exitNoButtonText.setString("Stay");
        exitNoButtonText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        exitNoButtonText.setFillColor(Color(255,255,255));
        FloatRect exitNoButtonTextRect = exitNoButtonText.getLocalBounds();
        exitNoButtonText.setOrigin(exitNoButtonTextRect.left+exitNoButtonTextRect.width/2.0f,exitNoButtonTextRect.top+exitNoButtonTextRect.height/2.0f);
        exitNoButtonText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + exitNoButton.getSize().x, VideoMode::getDesktopMode().height/2 + exitNoButton.getSize().y*1.2));

        //Size option:
        RectangleShape sizeMsg;
        sizeMsg.setSize(Vector2f(VideoMode::getDesktopMode().width/6, VideoMode::getDesktopMode().height / 10));
        sizeMsg.setOrigin(Vector2f(sizeMsg.getLocalBounds().width/2, sizeMsg.getGlobalBounds().height/2));
        sizeMsg.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));
        sizeMsg.setFillColor(Color(1,14,36,0));
        sizeMsg.setOutlineColor(Color(1,14,36,0));
        sizeMsg.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text sizeMsgText;
        sizeMsgText.setFont(font);
        sizeMsgText.setString("Select world size:");
        sizeMsgText.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        sizeMsgText.setFillColor(Color(255,255,255));
        FloatRect sizeMsgTextRect = sizeMsgText.getLocalBounds();
        sizeMsgText.setOrigin(sizeMsgTextRect.left+sizeMsgTextRect.width/2.0f,sizeMsgTextRect.top+sizeMsgTextRect.height/2.0f);
        sizeMsgText.setPosition(Vector2f(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2));
        sizeMsgText.setOutlineColor(Color(0,0,0));
        sizeMsgText.setOutlineThickness((VideoMode::getDesktopMode().width*8)/1920);

        //Optionssize 25
        RectangleShape size25;
        size25.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        size25.setOrigin(Vector2f(size25.getSize().x/2,size25.getSize().y/2));
        size25.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 - size25.getSize().x*2, VideoMode::getDesktopMode().height/2 + sizeMsg.getSize().y*1.2));
        size25.setFillColor(Color(113, 127, 128));
        size25.setOutlineColor(Color(255,255,255));
        size25.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text size25Text;
        size25Text.setFont(font);
        size25Text.setString("25x25");
        size25Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        size25Text.setFillColor(Color(255,255,255));
        FloatRect size25TextRect = size25Text.getLocalBounds();
        size25Text.setOrigin(size25TextRect.left+size25TextRect.width/2.0f,size25TextRect.top+size25TextRect.height/2.0f);
        size25Text.setPosition(size25.getPosition().x, size25.getPosition().y);

        //Optionssize 50
        RectangleShape size50;
        size50.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        size50.setOrigin(Vector2f(size50.getSize().x/2,size50.getSize().y/2));
        size50.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 - size50.getSize().x*0.7, VideoMode::getDesktopMode().height/2 + sizeMsg.getSize().y*1.2));
        size50.setFillColor(Color(113, 127, 128));
        size50.setOutlineColor(Color(255,255,255));
        size50.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text size50Text;
        size50Text.setFont(font);
        size50Text.setString("50x50");
        size50Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        size50Text.setFillColor(Color(255,255,255));
        FloatRect size50TextRect = size50Text.getLocalBounds();
        size50Text.setOrigin(size50TextRect.left+size50TextRect.width/2.0f,size50TextRect.top+size50TextRect.height/2.0f);
        size50Text.setPosition(size50.getPosition().x, size50.getPosition().y);

        //Optionssize 75
        RectangleShape size75;
        size75.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        size75.setOrigin(Vector2f(size75.getSize().x/2,size75.getSize().y/2));
        size75.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + size75.getSize().x*0.7, VideoMode::getDesktopMode().height/2 + sizeMsg.getSize().y*1.2));
        size75.setFillColor(Color(113, 127, 128));
        size75.setOutlineColor(Color(255,255,255));
        size75.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text size75Text;
        size75Text.setFont(font);
        size75Text.setString("75x75");
        size75Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        size75Text.setFillColor(Color(255,255,255));
        FloatRect size75TextRect = size75Text.getLocalBounds();
        size75Text.setOrigin(size75TextRect.left+size75TextRect.width/2.0f,size75TextRect.top+size75TextRect.height/2.0f);
        size75Text.setPosition(size75.getPosition().x, size75.getPosition().y);

        //Optionssize 100
        RectangleShape size100;
        size100.setSize(Vector2f(VideoMode::getDesktopMode().width/8, VideoMode::getDesktopMode().height / 20));
        size100.setOrigin(Vector2f(size100.getSize().x/2,size100.getSize().y/2));
        size100.setPosition(Vector2f(VideoMode::getDesktopMode().width/2 + size100.getSize().x*2, VideoMode::getDesktopMode().height/2 + sizeMsg.getSize().y*1.2));
        size100.setFillColor(Color(113, 127, 128));
        size100.setOutlineColor(Color(255,255,255));
        size100.setOutlineThickness((VideoMode::getDesktopMode().width*4)/1920);
        Text size100Text;
        size100Text.setFont(font);
        size100Text.setString("100x100");
        size100Text.setCharacterSize((VideoMode::getDesktopMode().width*60)/1920);
        size100Text.setFillColor(Color(255,255,255));
        FloatRect size100TextRect = size100Text.getLocalBounds();
        size100Text.setOrigin(size100TextRect.left+size100TextRect.width/2.0f,size100TextRect.top+size100TextRect.height/2.0f);
        size100Text.setPosition(size100.getPosition().x, size100.getPosition().y);

        while(menu.isOpen())
        {
            //std::cout << levelUpdateDelay << std::endl;

            while(menu.pollEvent(menuEvent))
            {
                if(menuEvent.type == Event::Closed)
                {
                    menu.close();
                    return -1;
                }
            }

            //check enter/escape presses:
            if(Keyboard::isKeyPressed(Keyboard::Enter) && inputBuffer.restart().asMilliseconds() >= inputDelay)
            {
                if(menuID == -1)
                {
                    menu.close();
                    return -1;
                }
            }
            if(Keyboard::isKeyPressed(Keyboard::Escape) && inputBuffer.restart().asMilliseconds() >= inputDelay)
            {
                if(menuID == 0)
                    menuID = -1;
                else if(menuID == 1)
                    menuID = 0;
                else if(menuID == 2)
                    menuID = 0;
                else if(menuID == 3)
                    menuID = 0;
                else if(menuID == 5)
                    menuID = 3;
            }

            //CheckButtonPressed:
            Vector2i mP = Mouse::getPosition(menu);
            if(menu.hasFocus())
            switch(menuID)
            {
            case -1:
                if(Mouse::isButtonPressed(Mouse::Left) && exitYesButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menu.close();
                    return -1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && exitNoButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;
                }
                break;

            case 0:
                if(Mouse::isButtonPressed(Mouse::Left) && exitButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = -1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && guideButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && optionsButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && playButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 3;
                }
                break;

            case 1:
                if(Mouse::isButtonPressed(Mouse::Left) && guideBackButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;
                }
                break;

            case 2:
                if(Mouse::isButtonPressed(Mouse::Left) && optionsBackButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;

                    tempFullscreen = Fullscreen;
                    tempWindowModeSizeX = windowModeSizeX;
                    tempWindowModeSizeY = windowModeSizeY;
                    tempMusicVolume = musicVolume;
                    tempSfxVolume = sfxVolume;
                    tempLevelUpdateDelay = levelUpdateDelay;
                    tempTexturePath = texturePath;

                    if(Fullscreen == false)
                    {
                        opFullscreenOff.setFillColor(Color(96, 184, 84));
                        opFullscreenOn.setFillColor(Color(113, 127, 128));
                    }
                    if(Fullscreen == true)
                    {
                        opFullscreenOff.setFillColor(Color(113, 127, 128));
                        opFullscreenOn.setFillColor(Color(96, 184, 84));
                    }

                    //Resolutions:
                    if(tempWindowModeSizeX == 1600 && tempWindowModeSizeY == 900)
                    {
                        res1600x900.setFillColor(Color(96, 184, 84));
                        res1366x768.setFillColor(Color(113, 127, 128));
                        res1024x576.setFillColor(Color(113, 127, 128));
                        res800x450.setFillColor(Color(113, 127, 128));
                    }
                    if(tempWindowModeSizeX == 1366 && tempWindowModeSizeY == 768)
                    {
                        res1600x900.setFillColor(Color(113, 127, 128));
                        res1366x768.setFillColor(Color(96, 184, 84));
                        res1024x576.setFillColor(Color(113, 127, 128));
                        res800x450.setFillColor(Color(113, 127, 128));
                    }
                    if(tempWindowModeSizeX == 1024 && tempWindowModeSizeY == 576)
                    {
                        res1600x900.setFillColor(Color(113, 127, 128));
                        res1366x768.setFillColor(Color(113, 127, 128));
                        res1024x576.setFillColor(Color(96, 184, 84));
                        res800x450.setFillColor(Color(113, 127, 128));
                    }
                    if(tempWindowModeSizeX == 800 && tempWindowModeSizeY == 450)
                    {
                        res1600x900.setFillColor(Color(113, 127, 128));
                        res1366x768.setFillColor(Color(113, 127, 128));
                        res1024x576.setFillColor(Color(113, 127, 128));
                        res800x450.setFillColor(Color(96, 184, 84));
                    }


                    //Music:
                    if(tempMusicVolume == 0)
                    {
                        musicVol0.setFillColor(Color(96, 184, 84));
                        musicVol25.setFillColor(Color(113, 127, 128));
                        musicVol50.setFillColor(Color(113, 127, 128));
                        musicVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempMusicVolume == 0.25)
                    {
                        musicVol0.setFillColor(Color(113, 127, 128));
                        musicVol25.setFillColor(Color(96, 184, 84));
                        musicVol50.setFillColor(Color(113, 127, 128));
                        musicVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempMusicVolume == 0.50)
                    {
                        musicVol0.setFillColor(Color(113, 127, 128));
                        musicVol25.setFillColor(Color(113, 127, 128));
                        musicVol50.setFillColor(Color(96, 184, 84));
                        musicVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempMusicVolume == 1)
                    {
                        musicVol0.setFillColor(Color(113, 127, 128));
                        musicVol25.setFillColor(Color(113, 127, 128));
                        musicVol50.setFillColor(Color(113, 127, 128));
                        musicVol100.setFillColor(Color(96, 184, 84));
                    }

                    //sfx:
                    if(tempSfxVolume == 0)
                    {
                        sfxVol0.setFillColor(Color(96, 184, 84));
                        sfxVol25.setFillColor(Color(113, 127, 128));
                        sfxVol50.setFillColor(Color(113, 127, 128));
                        sfxVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempSfxVolume == 0.25)
                    {
                        sfxVol0.setFillColor(Color(113, 127, 128));
                        sfxVol25.setFillColor(Color(96, 184, 84));
                        sfxVol50.setFillColor(Color(113, 127, 128));
                        sfxVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempSfxVolume == 0.50)
                    {
                        sfxVol0.setFillColor(Color(113, 127, 128));
                        sfxVol25.setFillColor(Color(113, 127, 128));
                        sfxVol50.setFillColor(Color(96, 184, 84));
                        sfxVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempSfxVolume == 1)
                    {
                        sfxVol0.setFillColor(Color(113, 127, 128));
                        sfxVol25.setFillColor(Color(113, 127, 128));
                        sfxVol50.setFillColor(Color(113, 127, 128));
                        sfxVol100.setFillColor(Color(96, 184, 84));
                    }

                    //LevelUpdate delay:
                    if(tempLevelUpdateDelay == (float)0.1)
                    {
                        delay01.setFillColor(Color(96, 184, 84));
                        delay05.setFillColor(Color(113, 127, 128));
                        delay10.setFillColor(Color(113, 127, 128));
                        delay20.setFillColor(Color(113, 127, 128));
                    }
                    if(tempLevelUpdateDelay == 0.5)
                    {
                        delay01.setFillColor(Color(113, 127, 128));
                        delay05.setFillColor(Color(96, 184, 84));
                        delay10.setFillColor(Color(113, 127, 128));
                        delay20.setFillColor(Color(113, 127, 128));
                    }
                    if(tempLevelUpdateDelay == 1)
                    {
                        delay01.setFillColor(Color(113, 127, 128));
                        delay05.setFillColor(Color(113, 127, 128));
                        delay10.setFillColor(Color(96, 184, 84));
                        delay20.setFillColor(Color(113, 127, 128));
                    }
                    if(tempLevelUpdateDelay == 2)
                    {
                        delay01.setFillColor(Color(113, 127, 128));
                        delay05.setFillColor(Color(113, 127, 128));
                        delay10.setFillColor(Color(113, 127, 128));
                        delay20.setFillColor(Color(96, 184, 84));
                    }

                    //Texture path:
                    if(tempTexturePath == "default")
                    {
                        txtDefault.setFillColor(Color(96, 184, 84));
                        txtDark.setFillColor(Color(113, 127, 128));
                        txtClassic.setFillColor(Color(113, 127, 128));
                        txtCustom.setFillColor(Color(113, 127, 128));
                    }
                    if(tempTexturePath == "dark")
                    {
                        txtDefault.setFillColor(Color(113, 127, 128));
                        txtDark.setFillColor(Color(96, 184, 84));
                        txtClassic.setFillColor(Color(113, 127, 128));
                        txtCustom.setFillColor(Color(113, 127, 128));
                    }
                    if(tempTexturePath == "classic")
                    {
                        txtDefault.setFillColor(Color(113, 127, 128));
                        txtDark.setFillColor(Color(113, 127, 128));
                        txtClassic.setFillColor(Color(96, 184, 84));
                        txtCustom.setFillColor(Color(113, 127, 128));
                    }
                    if(tempTexturePath == "custom")
                    {
                        txtDefault.setFillColor(Color(113, 127, 128));
                        txtDark.setFillColor(Color(113, 127, 128));
                        txtClassic.setFillColor(Color(113, 127, 128));
                        txtCustom.setFillColor(Color(96, 184, 84));
                    }

                }
                if(Mouse::isButtonPressed(Mouse::Left) && optionsApplyButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    Fullscreen = tempFullscreen;
                    windowModeSizeX = tempWindowModeSizeX;
                    windowModeSizeY = tempWindowModeSizeY;
                    musicVolume = tempMusicVolume;
                    sfxVolume = tempSfxVolume;
                    levelUpdateDelay = tempLevelUpdateDelay;
                    texturePath = tempTexturePath;

                    saveConfig();
                    menu.close();
                    menuID = 2;
                    goto Start;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && opFullscreenOn.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    opFullscreenOn.setFillColor(Color(96, 184, 84));
                    opFullscreenOff.setFillColor(Color(113, 127, 128));
                    tempFullscreen = true;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && opFullscreenOff.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    opFullscreenOn.setFillColor(Color(113, 127, 128));
                    opFullscreenOff.setFillColor(Color(96, 184, 84));
                    tempFullscreen = false;
                }
                //Resolution:
                if(Mouse::isButtonPressed(Mouse::Left) && res1600x900.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(96, 184, 84));
                    res1366x768.setFillColor(Color(113, 127, 128));
                    res1024x576.setFillColor(Color(113, 127, 128));
                    res800x450.setFillColor(Color(113, 127, 128));
                    tempWindowModeSizeX = 1600;
                    tempWindowModeSizeY = 900;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && res1366x768.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(113, 127, 128));
                    res1366x768.setFillColor(Color(96, 184, 84));
                    res1024x576.setFillColor(Color(113, 127, 128));
                    res800x450.setFillColor(Color(113, 127, 128));
                    tempWindowModeSizeX = 1366;
                    tempWindowModeSizeY = 768;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && res1024x576.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(113, 127, 128));
                    res1366x768.setFillColor(Color(113, 127, 128));
                    res1024x576.setFillColor(Color(96, 184, 84));
                    res800x450.setFillColor(Color(113, 127, 128));
                    tempWindowModeSizeX = 1024;
                    tempWindowModeSizeY = 576;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && res800x450.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(113, 127, 128));
                    res1366x768.setFillColor(Color(113, 127, 128));
                    res1024x576.setFillColor(Color(113, 127, 128));
                    res800x450.setFillColor(Color(96, 184, 84));
                    tempWindowModeSizeX = 800;
                    tempWindowModeSizeY = 450;
                }
                //Music:
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol0.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(96, 184, 84));
                    musicVol25.setFillColor(Color(113, 127, 128));
                    musicVol50.setFillColor(Color(113, 127, 128));
                    musicVol100.setFillColor(Color(113, 127, 128));
                    tempMusicVolume = 0;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol25.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(113, 127, 128));
                    musicVol25.setFillColor(Color(96, 184, 84));
                    musicVol50.setFillColor(Color(113, 127, 128));
                    musicVol100.setFillColor(Color(113, 127, 128));
                    tempMusicVolume = 0.25;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol50.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(113, 127, 128));
                    musicVol25.setFillColor(Color(113, 127, 128));
                    musicVol50.setFillColor(Color(96, 184, 84));
                    musicVol100.setFillColor(Color(113, 127, 128));
                    tempMusicVolume = 0.50;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol100.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(113, 127, 128));
                    musicVol25.setFillColor(Color(113, 127, 128));
                    musicVol50.setFillColor(Color(113, 127, 128));
                    musicVol100.setFillColor(Color(96, 184, 84));
                    tempMusicVolume = 1;
                }
                //SFX:
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol0.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(96, 184, 84));
                    sfxVol25.setFillColor(Color(113, 127, 128));
                    sfxVol50.setFillColor(Color(113, 127, 128));
                    sfxVol100.setFillColor(Color(113, 127, 128));
                    tempSfxVolume = 0;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol25.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(113, 127, 128));
                    sfxVol25.setFillColor(Color(96, 184, 84));
                    sfxVol50.setFillColor(Color(113, 127, 128));
                    sfxVol100.setFillColor(Color(113, 127, 128));
                    tempSfxVolume = 0.25;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol50.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(113, 127, 128));
                    sfxVol25.setFillColor(Color(113, 127, 128));
                    sfxVol50.setFillColor(Color(96, 184, 84));
                    sfxVol100.setFillColor(Color(113, 127, 128));
                    tempSfxVolume = 0.50;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol100.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(113, 127, 128));
                    sfxVol25.setFillColor(Color(113, 127, 128));
                    sfxVol50.setFillColor(Color(113, 127, 128));
                    sfxVol100.setFillColor(Color(96, 184, 84));
                    tempSfxVolume = 1;
                }
                //LevelUpdate delay:
                if(Mouse::isButtonPressed(Mouse::Left) && delay01.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(96, 184, 84));
                    delay05.setFillColor(Color(113, 127, 128));
                    delay10.setFillColor(Color(113, 127, 128));
                    delay20.setFillColor(Color(113, 127, 128));
                    tempLevelUpdateDelay = (float)0.1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && delay05.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(113, 127, 128));
                    delay05.setFillColor(Color(96, 184, 84));
                    delay10.setFillColor(Color(113, 127, 128));
                    delay20.setFillColor(Color(113, 127, 128));
                    tempLevelUpdateDelay = 0.5;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && delay10.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(113, 127, 128));
                    delay05.setFillColor(Color(113, 127, 128));
                    delay10.setFillColor(Color(96, 184, 84));
                    delay20.setFillColor(Color(113, 127, 128));
                    tempLevelUpdateDelay = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && delay20.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(113, 127, 128));
                    delay05.setFillColor(Color(113, 127, 128));
                    delay10.setFillColor(Color(113, 127, 128));
                    delay20.setFillColor(Color(96, 184, 84));
                    tempLevelUpdateDelay = 2;
                }

                //TexturePath :
                if(Mouse::isButtonPressed(Mouse::Left) && txtDefault.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(96, 184, 84));
                    txtDark.setFillColor(Color(113, 127, 128));
                    txtClassic.setFillColor(Color(113, 127, 128));
                    txtCustom.setFillColor(Color(113, 127, 128));
                    tempTexturePath = "default";
                }
                if(Mouse::isButtonPressed(Mouse::Left) && txtDark.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(113, 127, 128));
                    txtDark.setFillColor(Color(96, 184, 84));
                    txtClassic.setFillColor(Color(113, 127, 128));
                    txtCustom.setFillColor(Color(113, 127, 128));
                    tempTexturePath = "dark";
                }
                if(Mouse::isButtonPressed(Mouse::Left) && txtClassic.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(113, 127, 128));
                    txtDark.setFillColor(Color(113, 127, 128));
                    txtClassic.setFillColor(Color(96, 184, 84));
                    txtCustom.setFillColor(Color(113, 127, 128));
                    tempTexturePath = "classic";
                }
                if(Mouse::isButtonPressed(Mouse::Left) && txtCustom.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(113, 127, 128));
                    txtDark.setFillColor(Color(113, 127, 128));
                    txtClassic.setFillColor(Color(113, 127, 128));
                    txtCustom.setFillColor(Color(96, 184, 84));
                    tempTexturePath = "custom";
                }
                break;

            case 3:
                if(Mouse::isButtonPressed(Mouse::Left) && playBackButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot1.getGlobalBounds().contains(mP.x, mP.y) && is_slot1 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot2.getGlobalBounds().contains(mP.x, mP.y) && is_slot2 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot3.getGlobalBounds().contains(mP.x, mP.y) && is_slot3 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot4.getGlobalBounds().contains(mP.x, mP.y) && is_slot4 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 4;
                }

                //Create
                if(Mouse::isButtonPressed(Mouse::Left) && slot1DC.getGlobalBounds().contains(mP.x, mP.y) && is_slot1 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot2DC.getGlobalBounds().contains(mP.x, mP.y) && is_slot2 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot3DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot3 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot4DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot4 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 4;
                }

                //Delete
                if(Mouse::isButtonPressed(Mouse::Left) && slot1DC.getGlobalBounds().contains(mP.x, mP.y) && is_slot1 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot2DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot2 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot3DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot3 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot4DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot4 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 4;
                }
                break;

            case 4:
                if(saveSlot == 1)
                {
                    Sleep(2000);
                    menu.close();
                    return 1;
                }
                if(saveSlot == 2)
                {
                    Sleep(2000);
                    menu.close();
                    return 2;
                }
                if(saveSlot == 3)
                {
                    Sleep(2000);
                    menu.close();
                    return 3;
                }
                if(saveSlot == 4)
                {
                    Sleep(2000);
                    menu.close();
                    return 4;
                }
                break;

            case 5:
                if(Mouse::isButtonPressed(Mouse::Left) && deleteMsgNo.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    saveSlot = -1;
                    menuID = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && deleteMsgYes.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    if(saveSlot == 1)
                    {
                        is_slot1 = false;
                        deleteMap("slot1.map");
                    }
                    if(saveSlot == 2)
                    {
                        is_slot2 = false;
                        deleteMap("slot2.map");
                    }
                    if(saveSlot == 3)
                    {
                        is_slot3 = false;
                        deleteMap("slot3.map");
                    }
                    if(saveSlot == 4)
                    {
                        is_slot4 = false;
                        deleteMap("slot4.map");
                    }

                    menuID = 3;
                    goto Start;
                }
                break;

            case 6:
                if(Mouse::isButtonPressed(Mouse::Left) && size25.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 25;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && size50.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 50;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && size75.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 75;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && size100.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 100;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                break;
            }

            //Drawing:
            menu.clear(Color(1,14,36));

            switch(menuID)
            {
            case -1:
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(exitConfirmationMessage);
                menu.draw(exitConfirmationText);

                menu.draw(exitYesButton);
                menu.draw(exitYesButtonText);

                menu.draw(exitNoButton);
                menu.draw(exitNoButtonText);
                break;
            case 0:
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(logoText);
                menu.draw(infoText);

                menu.draw(playButton);
                menu.draw(playButtonText);

                menu.draw(guideButton);
                menu.draw(guideButtonText);

                menu.draw(optionsButton);
                menu.draw(optionsButtonText);

                menu.draw(exitButton);
                menu.draw(exitButtonText);
                break;

            case 1:
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(guideMessageText);

                menu.draw(guideBackButton);
                menu.draw(guideBackText);
                break;

            case 2:
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(optionsMessage);

                menu.draw(opFullscreenOn);
                menu.draw(opFullscreenOnText);
                menu.draw(opFullscreenOff);
                menu.draw(opFullscreenOffText);

                menu.draw(res1600x900);
                menu.draw(res1600x900Text);
                menu.draw(res1366x768);
                menu.draw(res1366x768Text);
                menu.draw(res1024x576);
                menu.draw(res1024x576Text);
                menu.draw(res800x450);
                menu.draw(res800x450Text);

                menu.draw(musicVol0);
                menu.draw(musicVol0Text);
                menu.draw(musicVol25);
                menu.draw(musicVol25Text);
                menu.draw(musicVol50);
                menu.draw(musicVol50Text);
                menu.draw(musicVol100);
                menu.draw(musicVol100Text);

                menu.draw(sfxVol0);
                menu.draw(sfxVol0Text);
                menu.draw(sfxVol25);
                menu.draw(sfxVol25Text);
                menu.draw(sfxVol50);
                menu.draw(sfxVol50Text);
                menu.draw(sfxVol100);
                menu.draw(sfxVol100Text);

                menu.draw(delay01);
                menu.draw(delay01Text);
                menu.draw(delay05);
                menu.draw(delay05Text);
                menu.draw(delay10);
                menu.draw(delay10Text);
                menu.draw(delay20);
                menu.draw(delay20Text);

                menu.draw(txtDefault);
                menu.draw(txtDefaultText);
                menu.draw(txtDark);
                menu.draw(txtDarkText);
                menu.draw(txtClassic);
                menu.draw(txtClassicText);
                menu.draw(txtCustom);
                menu.draw(txtCustomText);

                menu.draw(optionsBackButton);
                menu.draw(optionsBackText);

                menu.draw(optionsApplyButton);
                menu.draw(optionsApplyText);
                break;

            case 3 :
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(playBackButton);
                menu.draw(playBackText);

                menu.draw(slot1);
                menu.draw(slot1Text);
                menu.draw(slot1DC);
                menu.draw(slot1DCText);

                menu.draw(slot2);
                menu.draw(slot2Text);
                menu.draw(slot2DC);
                menu.draw(slot2DCText);

                menu.draw(slot3);
                menu.draw(slot3Text);
                menu.draw(slot3DC);
                menu.draw(slot3DCText);

                menu.draw(slot4);
                menu.draw(slot4Text);
                menu.draw(slot4DC);
                menu.draw(slot4DCText);

                break;
            case 4:
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(loadingText);

                break;

            case 5:
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(deleteMsg);
                menu.draw(deleteMsgText);

                menu.draw(deleteMsgYes);
                menu.draw(deleteMsgYesText);

                menu.draw(deleteMsgNo);
                menu.draw(deleteMsgNoText);
                break;

            case 6:
                //Draw Background
                for(int x = 0; x < VideoMode::getDesktopMode().width; x+=64)
                {
                    for(int y = 0; y < VideoMode::getDesktopMode().height; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }


                menu.draw(sizeMsg);
                menu.draw(sizeMsgText);

                menu.draw(size25);
                menu.draw(size25Text);
                menu.draw(size50);
                menu.draw(size50Text);
                menu.draw(size75);
                menu.draw(size75Text);
                menu.draw(size100);
                menu.draw(size100Text);
                break;

            }

            menu.display();
        }
    }

    else if(Fullscreen == false)
    {

        menu.create(VideoMode(windowModeSizeX, windowModeSizeY), "Computation", Style::Close);
        menu.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        menu.setFramerateLimit(100);

        //Logo
        Text logoText;
        logoText.setFont(logoFont);
        logoText.setString("COMPUTATION");
        logoText.setCharacterSize((windowModeSizeX*200)/1920);
        logoText.setFillColor(Color(255,255,255));
        logoText.setOutlineColor(Color(0,0,0));
        logoText.setOutlineThickness((windowModeSizeX*15)/1920);
        FloatRect logoTextRect = logoText.getLocalBounds();
        logoText.setOrigin(logoTextRect.left+logoTextRect.width/2.0f,logoTextRect.top+logoTextRect.height/2.0f);
        logoText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/4));

        //info:
        Text infoText;
        infoText.setFont(font);
        infoText.setString("Made by Ene Alexandru & Alexe Paul\nVersion: " + versionNumber);
        infoText.setCharacterSize((windowModeSizeX*15)/1920);
        infoText.setFillColor(Color(255,255,255));
        infoText.setOutlineColor(Color(0,0,0));
        infoText.setOutlineThickness((windowModeSizeX*4)/1920);
        infoText.setPosition(Vector2f(0,0));

        //Play button:
        RectangleShape playButton;
        playButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        playButton.setOrigin(Vector2f(playButton.getLocalBounds().width/2, playButton.getGlobalBounds().height/2));
        playButton.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));
        playButton.setFillColor(Color(255, 182, 64));
        playButton.setOutlineColor(Color(255,255,255));
        playButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text playButtonText;
        playButtonText.setFont(font);
        playButtonText.setString("PLAY");
        playButtonText.setCharacterSize((windowModeSizeX*100)/1920);
        playButtonText.setFillColor(Color(0,0,0));
        FloatRect playButtonTextRect = playButtonText.getLocalBounds();
        playButtonText.setOrigin(playButtonTextRect.left+playButtonTextRect.width/2.0f,playButtonTextRect.top+playButtonTextRect.height/2.0f);
        playButtonText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));

        //PlayBackButton:
        RectangleShape playBackButton;
        playBackButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        playBackButton.setOrigin(Vector2f(playBackButton.getLocalBounds().width/2, playBackButton.getGlobalBounds().height/2));
        playBackButton.setPosition(Vector2f(windowModeSizeX/2 + playBackButton.getSize().x*1.2*2, windowModeSizeY/2 + playBackButton.getSize().y*1.2*3));
        playBackButton.setFillColor(Color(222, 55, 55));
        playBackButton.setOutlineColor(Color(255,255,255));
        playBackButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text playBackText;
        playBackText.setFont(font);
        playBackText.setString("Back");
        playBackText.setCharacterSize((windowModeSizeX*60)/1920);
        playBackText.setFillColor(Color(255,255,255));
        FloatRect playBackTextRect = playBackText.getLocalBounds();
        playBackText.setOrigin(playBackTextRect.left+playBackTextRect.width/2.0f,playBackTextRect.top+playBackTextRect.height/2.0f);
        playBackText.setPosition(Vector2f(windowModeSizeX/2 + playBackButton.getSize().x*1.2*2, windowModeSizeY/2 + playBackButton.getSize().y*1.2*3));

        //Slot1:
        RectangleShape slot1;
        slot1.setSize(Vector2f(windowModeSizeX/4, windowModeSizeY / 10));
        slot1.setPosition(Vector2f(windowModeSizeX/10, windowModeSizeY/10));
        slot1.setOutlineColor(Color(255,255,255));
        slot1.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot1Text;
        slot1Text.setFont(font);
        slot1Text.setCharacterSize((windowModeSizeX*60)/1920);
        slot1Text.setFillColor(Color(255,255,255));
        if(is_slot1 == true)
        {
            slot1.setFillColor(Color(96, 184, 84));
            slot1Text.setString("Save slot #1");
        }
        else if(is_slot1 != true)
        {
            slot1.setFillColor(Color(113, 127, 128));
            slot1Text.setString("Empty");
        }
        FloatRect slot1TextRect = slot1Text.getLocalBounds();
        slot1Text.setOrigin(slot1TextRect.left+slot1TextRect.width/2.0f,slot1TextRect.top+slot1TextRect.height/2.0f);
        slot1Text.setPosition(slot1.getPosition().x+slot1.getSize().x/2, slot1.getPosition().y+slot1.getSize().y/2);

        //Slot1 delete/crate
        RectangleShape slot1DC;
        slot1DC.setSize(Vector2f(windowModeSizeX/10, windowModeSizeY / 10));
        slot1DC.setPosition(Vector2f(windowModeSizeX/10 + slot1.getSize().x*1.1, windowModeSizeY/10));
        slot1DC.setOutlineColor(Color(255,255,255));
        slot1DC.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot1DCText;
        slot1DCText.setFont(font);
        slot1DCText.setCharacterSize((windowModeSizeX*60)/1920);
        slot1DCText.setFillColor(Color(255,255,255));
        if(is_slot1 == true)
        {
            slot1DC.setFillColor(Color(222, 55, 55));
            slot1DCText.setString("Delete");
        }
        else if(is_slot1 != true)
        {
            slot1DC.setFillColor(Color(96,184,84));
            slot1DCText.setString("Create");
        }
        FloatRect slot1DCTextRect = slot1DCText.getLocalBounds();
        slot1DCText.setOrigin(slot1DCTextRect.left+slot1DCTextRect.width/2.0f,slot1DCTextRect.top+slot1DCTextRect.height/2.0f);
        slot1DCText.setPosition(slot1DC.getPosition().x+slot1DC.getSize().x/2, slot1DC.getPosition().y+slot1DC.getSize().y/2);


        //Slot2:
        RectangleShape slot2;
        slot2.setSize(Vector2f(windowModeSizeX/4, windowModeSizeY / 10));
        slot2.setPosition(Vector2f(windowModeSizeX/10, windowModeSizeY/10 + slot2.getSize().y*1.3*1));
        slot2.setOutlineColor(Color(255,255,255));
        slot2.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot2Text;
        slot2Text.setFont(font);
        slot2Text.setCharacterSize((windowModeSizeX*60)/1920);
        slot2Text.setFillColor(Color(255,255,255));
        if(is_slot2 == true)
        {
            slot2.setFillColor(Color(96, 184, 84));
            slot2Text.setString("Save slot #2");
        }
        else if(is_slot2 != true)
        {
            slot2.setFillColor(Color(113, 127, 128));
            slot2Text.setString("Empty");
        }
        FloatRect slot2TextRect = slot2Text.getLocalBounds();
        slot2Text.setOrigin(slot2TextRect.left+slot2TextRect.width/2.0f,slot2TextRect.top+slot2TextRect.height/2.0f);
        slot2Text.setPosition(slot2.getPosition().x+slot2.getSize().x/2, slot2.getPosition().y+slot2.getSize().y/2);

        //Slot2 delete/crate
        RectangleShape slot2DC;
        slot2DC.setSize(Vector2f(windowModeSizeX/10, windowModeSizeY / 10));
        slot2DC.setPosition(Vector2f(windowModeSizeX/10 + slot2.getSize().x*1.1, windowModeSizeY/10 + slot2.getSize().y*1.3*1));
        slot2DC.setOutlineColor(Color(255,255,255));
        slot2DC.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot2DCText;
        slot2DCText.setFont(font);
        slot2DCText.setCharacterSize((windowModeSizeX*60)/1920);
        slot2DCText.setFillColor(Color(255,255,255));
        if(is_slot2 == true)
        {
            slot2DC.setFillColor(Color(222, 55, 55));
            slot2DCText.setString("Delete");
        }
        else if(is_slot2 != true)
        {
            slot2DC.setFillColor(Color(96,184,84));
            slot2DCText.setString("Create");
        }
        FloatRect slot2DCTextRect = slot2DCText.getLocalBounds();
        slot2DCText.setOrigin(slot2DCTextRect.left+slot2DCTextRect.width/2.0f,slot2DCTextRect.top+slot2DCTextRect.height/2.0f);
        slot2DCText.setPosition(slot2DC.getPosition().x+slot2DC.getSize().x/2, slot2DC.getPosition().y+slot2DC.getSize().y/2);

        //Slot3:
        RectangleShape slot3;
        slot3.setSize(Vector2f(windowModeSizeX/4, windowModeSizeY / 10));
        slot3.setPosition(Vector2f(windowModeSizeX/10, windowModeSizeY/10 + slot3.getSize().y*1.3*2));
        slot3.setOutlineColor(Color(255,255,255));
        slot3.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot3Text;
        slot3Text.setFont(font);
        slot3Text.setCharacterSize((windowModeSizeX*60)/1920);
        slot3Text.setFillColor(Color(255,255,255));
        if(is_slot3 == true)
        {
            slot3.setFillColor(Color(96, 184, 84));
            slot3Text.setString("Save slot #3");
        }
        else if(is_slot3 != true)
        {
            slot3.setFillColor(Color(113, 127, 128));
            slot3Text.setString("Empty");
        }
        FloatRect slot3TextRect = slot3Text.getLocalBounds();
        slot3Text.setOrigin(slot3TextRect.left+slot3TextRect.width/2.0f,slot3TextRect.top+slot3TextRect.height/2.0f);
        slot3Text.setPosition(slot3.getPosition().x+slot3.getSize().x/2, slot3.getPosition().y+slot3.getSize().y/2);

        //Slot3 delete/crate
        RectangleShape slot3DC;
        slot3DC.setSize(Vector2f(windowModeSizeX/10, windowModeSizeY / 10));
        slot3DC.setPosition(Vector2f(windowModeSizeX/10 + slot3.getSize().x*1.1, windowModeSizeY/10 + slot3.getSize().y*1.3*2));
        slot3DC.setOutlineColor(Color(255,255,255));
        slot3DC.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot3DCText;
        slot3DCText.setFont(font);
        slot3DCText.setCharacterSize((windowModeSizeX*60)/1920);
        slot3DCText.setFillColor(Color(255,255,255));
        if(is_slot3 == true)
        {
            slot3DC.setFillColor(Color(222, 55, 55));
            slot3DCText.setString("Delete");
        }
        else if(is_slot3 != true)
        {
            slot3DC.setFillColor(Color(96,184,84));
            slot3DCText.setString("Create");
        }
        FloatRect slot3DCTextRect = slot3DCText.getLocalBounds();
        slot3DCText.setOrigin(slot3DCTextRect.left+slot3DCTextRect.width/2.0f,slot3DCTextRect.top+slot3DCTextRect.height/2.0f);
        slot3DCText.setPosition(slot3DC.getPosition().x+slot3DC.getSize().x/2, slot3DC.getPosition().y+slot3DC.getSize().y/2);

        //Slot4:
        RectangleShape slot4;
        slot4.setSize(Vector2f(windowModeSizeX/4, windowModeSizeY / 10));
        slot4.setPosition(Vector2f(windowModeSizeX/10, windowModeSizeY/10 + slot4.getSize().y*1.3*3));
        slot4.setOutlineColor(Color(255,255,255));
        slot4.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot4Text;
        slot4Text.setFont(font);
        slot4Text.setCharacterSize((windowModeSizeX*60)/1920);
        slot4Text.setFillColor(Color(255,255,255));
        if(is_slot4 == true)
        {
            slot4.setFillColor(Color(96, 184, 84));
            slot4Text.setString("Save slot #4");
        }
        else if(is_slot4 != true)
        {
            slot4.setFillColor(Color(113, 127, 128));
            slot4Text.setString("Empty");
        }
        FloatRect slot4TextRect = slot4Text.getLocalBounds();
        slot4Text.setOrigin(slot4TextRect.left+slot4TextRect.width/2.0f,slot4TextRect.top+slot4TextRect.height/2.0f);
        slot4Text.setPosition(slot4.getPosition().x+slot4.getSize().x/2, slot4.getPosition().y+slot4.getSize().y/2);

        //Slot4 delete/crate
        RectangleShape slot4DC;
        slot4DC.setSize(Vector2f(windowModeSizeX/10, windowModeSizeY / 10));
        slot4DC.setPosition(Vector2f(windowModeSizeX/10 + slot4.getSize().x*1.1, windowModeSizeY/10 + slot4.getSize().y*1.3*3));
        slot4DC.setOutlineColor(Color(255,255,255));
        slot4DC.setOutlineThickness((windowModeSizeX*4)/1920);
        Text slot4DCText;
        slot4DCText.setFont(font);
        slot4DCText.setCharacterSize((windowModeSizeX*60)/1920);
        slot4DCText.setFillColor(Color(255,255,255));
        if(is_slot4 == true)
        {
            slot4DC.setFillColor(Color(222, 55, 55));
            slot4DCText.setString("Delete");
        }
        else if(is_slot4 != true)
        {
            slot4DC.setFillColor(Color(96,184,84));
            slot4DCText.setString("Create");
        }
        FloatRect slot4DCTextRect = slot4DCText.getLocalBounds();
        slot4DCText.setOrigin(slot4DCTextRect.left+slot4DCTextRect.width/2.0f,slot4DCTextRect.top+slot4DCTextRect.height/2.0f);
        slot4DCText.setPosition(slot4DC.getPosition().x+slot4DC.getSize().x/2, slot4DC.getPosition().y+slot4DC.getSize().y/2);

        //LoadingScreen
        Text loadingText;
        loadingText.setFont(font);
        loadingText.setString("Loading...");
        loadingText.setCharacterSize((windowModeSizeX*60)/1920);
        loadingText.setFillColor(Color(255,255,255));
        loadingText.setOutlineColor(Color(0,0,0));
        loadingText.setOutlineThickness((windowModeSizeX*4)/1920);
        FloatRect loadingTextRect = loadingText.getLocalBounds();
        loadingText.setOrigin(loadingTextRect.left+loadingTextRect.width/2.0f,loadingTextRect.top+loadingTextRect.height/2.0f);
        loadingText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));

        //DeleteConfirmationMessage:
        RectangleShape deleteMsg;
        deleteMsg.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        deleteMsg.setOrigin(Vector2f(deleteMsg.getLocalBounds().width/2, deleteMsg.getGlobalBounds().height/2));
        deleteMsg.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));
        deleteMsg.setFillColor(Color(1,14,36,0));
        deleteMsg.setOutlineColor(Color(1,14,36,0));
        deleteMsg.setOutlineThickness((windowModeSizeX*4)/1920);
        Text deleteMsgText;
        deleteMsgText.setFont(font);
        deleteMsgText.setString("Are you sure you want to delete this save?");
        deleteMsgText.setCharacterSize((windowModeSizeX*60)/1920);
        deleteMsgText.setFillColor(Color(255,255,255));
        FloatRect deleteMsgTextRect = deleteMsgText.getLocalBounds();
        deleteMsgText.setOrigin(deleteMsgTextRect.left+deleteMsgTextRect.width/2.0f,deleteMsgTextRect.top+deleteMsgTextRect.height/2.0f);
        deleteMsgText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));
        deleteMsgText.setOutlineColor(Color(0,0,0));
        deleteMsgText.setOutlineThickness((windowModeSizeX*8)/1920);


        //DeleteConfirmationMessageYES:
        RectangleShape deleteMsgYes;
        deleteMsgYes.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        deleteMsgYes.setOrigin(Vector2f(deleteMsgYes.getLocalBounds().width/2, deleteMsgYes.getGlobalBounds().height/2));
        deleteMsgYes.setPosition(Vector2f(windowModeSizeX/2 - deleteMsgYes.getSize().x, windowModeSizeY/2 + deleteMsgYes.getSize().y*1.2));
        deleteMsgYes.setFillColor(Color(222, 55, 55));
        deleteMsgYes.setOutlineColor(Color(255,255,255));
        deleteMsgYes.setOutlineThickness((windowModeSizeX*4)/1920);
        Text deleteMsgYesText;
        deleteMsgYesText.setFont(font);
        deleteMsgYesText.setString("Delete");
        deleteMsgYesText.setCharacterSize((windowModeSizeX*60)/1920);
        deleteMsgYesText.setFillColor(Color(255,255,255));
        FloatRect deleteMsgYesTextRect = deleteMsgYesText.getLocalBounds();
        deleteMsgYesText.setOrigin(deleteMsgYesTextRect.left+deleteMsgYesTextRect.width/2.0f,deleteMsgYesTextRect.top+deleteMsgYesTextRect.height/2.0f);
        deleteMsgYesText.setPosition(Vector2f(windowModeSizeX/2 - deleteMsgYes.getSize().x, windowModeSizeY/2 + deleteMsgYes.getSize().y*1.2));

        //DeleteConfirmationMessageNo:
        RectangleShape deleteMsgNo;
        deleteMsgNo.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        deleteMsgNo.setOrigin(Vector2f(deleteMsgNo.getLocalBounds().width/2, deleteMsgNo.getGlobalBounds().height/2));
        deleteMsgNo.setPosition(Vector2f(windowModeSizeX/2 + deleteMsgNo.getSize().x, windowModeSizeY/2 + deleteMsgNo.getSize().y*1.2));
        deleteMsgNo.setFillColor(Color(96, 184, 84));
        deleteMsgNo.setOutlineColor(Color(255,255,255));
        deleteMsgNo.setOutlineThickness((windowModeSizeX*4)/1920);
        Text deleteMsgNoText;
        deleteMsgNoText.setFont(font);
        deleteMsgNoText.setString("Cancel");
        deleteMsgNoText.setCharacterSize((windowModeSizeX*60)/1920);
        deleteMsgNoText.setFillColor(Color(255,255,255));
        FloatRect deleteMsgNoTextRect = deleteMsgNoText.getLocalBounds();
        deleteMsgNoText.setOrigin(deleteMsgNoTextRect.left+deleteMsgNoTextRect.width/2.0f,deleteMsgNoTextRect.top+deleteMsgNoTextRect.height/2.0f);
        deleteMsgNoText.setPosition(Vector2f(windowModeSizeX/2 + deleteMsgNo.getSize().x, windowModeSizeY/2 + deleteMsgNo.getSize().y*1.2));

        //Guide Button:
        RectangleShape guideButton;
        guideButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        guideButton.setOrigin(Vector2f(guideButton.getLocalBounds().width/2, guideButton.getGlobalBounds().height/2));
        guideButton.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2 + guideButton.getSize().y*1.2));
        guideButton.setFillColor(Color(20, 206, 219));
        guideButton.setOutlineColor(Color(255,255,255));
        guideButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text guideButtonText;
        guideButtonText.setFont(font);
        guideButtonText.setString("Guide");
        guideButtonText.setCharacterSize((windowModeSizeX*60)/1920);
        guideButtonText.setFillColor(Color(255,255,255));
        FloatRect guideButtonTextRect = guideButtonText.getLocalBounds();
        guideButtonText.setOrigin(guideButtonTextRect.left+guideButtonTextRect.width/2.0f,guideButtonTextRect.top+guideButtonTextRect.height/2.0f);
        guideButtonText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2 + guideButton.getSize().y*1.2));

        //GuideBackButton:
        RectangleShape guideBackButton;
        guideBackButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        guideBackButton.setOrigin(Vector2f(guideBackButton.getLocalBounds().width/2, guideBackButton.getGlobalBounds().height/2));
        guideBackButton.setPosition(Vector2f(windowModeSizeX/2 + guideBackButton.getSize().x*1.2*2, windowModeSizeY/2 + guideBackButton.getSize().y*1.2*3));
        guideBackButton.setFillColor(Color(222, 55, 55));
        guideBackButton.setOutlineColor(Color(255,255,255));
        guideBackButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text guideBackText;
        guideBackText.setFont(font);
        guideBackText.setString("Back");
        guideBackText.setCharacterSize((windowModeSizeX*60)/1920);
        guideBackText.setFillColor(Color(255,255,255));
        FloatRect guideBackTextRect = guideBackText.getLocalBounds();
        guideBackText.setOrigin(guideBackTextRect.left+guideBackTextRect.width/2.0f,guideBackTextRect.top+guideBackTextRect.height/2.0f);
        guideBackText.setPosition(Vector2f(windowModeSizeX/2 + guideBackButton.getSize().x*1.2*2, windowModeSizeY/2 + guideBackButton.getSize().y*1.2*3));

        //Guide Message:
        Text guideMessageText;
        guideMessageText.setFont(font);
        guideMessageText.setString("Press 'Escape' to open the PAUSE menu.\nPress 'F11' to toggle Fullscreen.\nPress 'I' to open the inventory.\nPress 'W'/'A'/'S'/'D' to move the camera around.\nPress 'Ctrl' + '+'/'-' to change zoom level.\nPress 'Q' to pick a block from the ground.\nPress 'R' to rotate the block in hand.\nPress 'Left Mouse Click' to place blocks\n\nPlacing an empty block on the ground will remove the already existing block.\nRefer to the guide from the game folder for more information regarding each block.");
        guideMessageText.setCharacterSize((windowModeSizeX*40)/1920);
        guideMessageText.setFillColor(Color(255,255,255));
        guideMessageText.setPosition(Vector2f(windowModeSizeX/10, windowModeSizeY/10));
        guideMessageText.setOutlineColor(Color(0,0,0));
        guideMessageText.setOutlineThickness((windowModeSizeX*8)/1920);

        //Options Button:
        RectangleShape optionsButton;
        optionsButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        optionsButton.setOrigin(Vector2f(optionsButton.getLocalBounds().width/2, optionsButton.getGlobalBounds().height/2));
        optionsButton.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2 + optionsButton.getSize().y*1.2*2));
        optionsButton.setFillColor(Color(113, 127, 128));
        optionsButton.setOutlineColor(Color(255,255,255));
        optionsButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text optionsButtonText;
        optionsButtonText.setFont(font);
        optionsButtonText.setString("Options");
        optionsButtonText.setCharacterSize((windowModeSizeX*60)/1920);
        optionsButtonText.setFillColor(Color(255,255,255));
        FloatRect optionsButtonTextRect = optionsButtonText.getLocalBounds();
        optionsButtonText.setOrigin(optionsButtonTextRect.left+optionsButtonTextRect.width/2.0f,optionsButtonTextRect.top+optionsButtonTextRect.height/2.0f);
        optionsButtonText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2 + optionsButton.getSize().y*1.2*2));

        //OptionsBackButton:
        RectangleShape optionsBackButton;
        optionsBackButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        optionsBackButton.setOrigin(Vector2f(optionsBackButton.getLocalBounds().width/2, optionsBackButton.getGlobalBounds().height/2));
        optionsBackButton.setPosition(Vector2f(windowModeSizeX/2 + optionsBackButton.getSize().x*1.2*2, windowModeSizeY/2 + optionsBackButton.getSize().y*1.2*3));
        optionsBackButton.setFillColor(Color(222, 55, 55));
        optionsBackButton.setOutlineColor(Color(255,255,255));
        optionsBackButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text optionsBackText;
        optionsBackText.setFont(font);
        optionsBackText.setString("Back");
        optionsBackText.setCharacterSize((windowModeSizeX*60)/1920);
        optionsBackText.setFillColor(Color(255,255,255));
        FloatRect optionsBackTextRect = optionsBackText.getLocalBounds();
        optionsBackText.setOrigin(optionsBackTextRect.left+optionsBackTextRect.width/2.0f,optionsBackTextRect.top+optionsBackTextRect.height/2.0f);
        optionsBackText.setPosition(Vector2f(windowModeSizeX/2 + optionsBackButton.getSize().x*1.2*2, windowModeSizeY/2 + optionsBackButton.getSize().y*1.2*3));

        //OptionsApplyButton:
        RectangleShape optionsApplyButton;
        optionsApplyButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        optionsApplyButton.setOrigin(Vector2f(optionsApplyButton.getLocalBounds().width/2, optionsApplyButton.getGlobalBounds().height/2));
        optionsApplyButton.setPosition(Vector2f(windowModeSizeX/2 + optionsApplyButton.getSize().x*1.2*1, windowModeSizeY/2 + optionsApplyButton.getSize().y*1.2*3));
        optionsApplyButton.setFillColor(Color(96, 184, 84));
        optionsApplyButton.setOutlineColor(Color(255,255,255));
        optionsApplyButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text optionsApplyText;
        optionsApplyText.setFont(font);
        optionsApplyText.setString("Apply");
        optionsApplyText.setCharacterSize((windowModeSizeX*60)/1920);
        optionsApplyText.setFillColor(Color(255,255,255));
        FloatRect optionsApplyTextRect = optionsApplyText.getLocalBounds();
        optionsApplyText.setOrigin(optionsApplyTextRect.left+optionsApplyTextRect.width/2.0f,optionsApplyTextRect.top+optionsApplyTextRect.height/2.0f);
        optionsApplyText.setPosition(Vector2f(windowModeSizeX/2 + optionsApplyButton.getSize().x*1.2*1, windowModeSizeY/2 + optionsApplyButton.getSize().y*1.2*3));

        //OptionsMessage:
        Text optionsMessage;
        optionsMessage.setFont(font);
        optionsMessage.setString("Fullscreen           :\nResolution           :\nMusic volume         :\nSFX volume           :\nLevel update delay   :\nTexturePack          :");
        optionsMessage.setCharacterSize((windowModeSizeX*40)/1920);
        optionsMessage.setFillColor(Color(255,255,255));
        optionsMessage.setPosition(Vector2f(windowModeSizeX/10, windowModeSizeY/10));
        optionsMessage.setOutlineColor(Color(0,0,0));
        optionsMessage.setOutlineThickness((windowModeSizeX*8)/1920);
        optionsMessage.setLineSpacing(3);

        //OptionFullscreenOn:
        RectangleShape opFullscreenOn;
        opFullscreenOn.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        opFullscreenOn.setPosition(Vector2f(windowModeSizeX/2.5, windowModeSizeY/10));
        if(tempFullscreen == true)
            opFullscreenOn.setFillColor(Color(96, 184, 84));
        else if(tempFullscreen == false)
            opFullscreenOn.setFillColor(Color(113, 127, 128));
        opFullscreenOn.setOutlineColor(Color(255,255,255));
        opFullscreenOn.setOutlineThickness((windowModeSizeX*4)/1920);
        Text opFullscreenOnText;
        opFullscreenOnText.setFont(font);
        opFullscreenOnText.setString("On");
        opFullscreenOnText.setCharacterSize((windowModeSizeX*60)/1920);
        opFullscreenOnText.setFillColor(Color(255,255,255));
        FloatRect opFullScreenOnTextRect = opFullscreenOnText.getLocalBounds();
        opFullscreenOnText.setOrigin(opFullScreenOnTextRect.left+opFullScreenOnTextRect.width/2.0f,opFullScreenOnTextRect.top+opFullScreenOnTextRect.height/2.0f);
        opFullscreenOnText.setPosition(opFullscreenOn.getPosition().x+opFullscreenOn.getSize().x/2, opFullscreenOn.getPosition().y+opFullscreenOn.getSize().y/2);

        //Option FullscreenOFF:
        RectangleShape opFullscreenOff;
        opFullscreenOff.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        opFullscreenOff.setPosition(Vector2f(windowModeSizeX/2.5 + opFullscreenOff.getSize().x*1.2, windowModeSizeY/10));
        if(tempFullscreen == false)
            opFullscreenOff.setFillColor(Color(96, 184, 84));
        else if(tempFullscreen == true)
            opFullscreenOff.setFillColor(Color(113, 127, 128));
        opFullscreenOff.setOutlineColor(Color(255,255,255));
        opFullscreenOff.setOutlineThickness((windowModeSizeX*4)/1920);
        Text opFullscreenOffText;
        opFullscreenOffText.setFont(font);
        opFullscreenOffText.setString("Off");
        opFullscreenOffText.setCharacterSize((windowModeSizeX*60)/1920);
        opFullscreenOffText.setFillColor(Color(255,255,255));
        FloatRect opFullscreenOffTextRect = opFullscreenOffText.getLocalBounds();
        opFullscreenOffText.setOrigin(opFullscreenOffTextRect.left+opFullscreenOffTextRect.width/2.0f,opFullscreenOffTextRect.top+opFullscreenOffTextRect.height/2.0f);
        opFullscreenOffText.setPosition(opFullscreenOff.getPosition().x+opFullscreenOff.getSize().x/2, opFullscreenOff.getPosition().y+opFullscreenOff.getSize().y/2);

        //Options res1600x900
        RectangleShape res1600x900;
        res1600x900.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        res1600x900.setPosition(Vector2f(windowModeSizeX/2.5 + res1600x900.getSize().x*1.2*0, windowModeSizeY/10 + res1600x900.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 1600 && tempWindowModeSizeY == 900)
            res1600x900.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 1600 && tempWindowModeSizeY != 900)
            res1600x900.setFillColor(Color(113, 127, 128));
        res1600x900.setOutlineColor(Color(255,255,255));
        res1600x900.setOutlineThickness((windowModeSizeX*4)/1920);
        Text res1600x900Text;
        res1600x900Text.setFont(font);
        res1600x900Text.setString("1600x900");
        res1600x900Text.setCharacterSize((windowModeSizeX*60)/1920);
        res1600x900Text.setFillColor(Color(255,255,255));
        FloatRect res1600x900TextRect = res1600x900Text.getLocalBounds();
        res1600x900Text.setOrigin(res1600x900TextRect.left+res1600x900TextRect.width/2.0f,res1600x900TextRect.top+res1600x900TextRect.height/2.0f);
        res1600x900Text.setPosition(res1600x900.getPosition().x+res1600x900.getSize().x/2, res1600x900.getPosition().y+res1600x900.getSize().y/2);

        //Options res1366x768
        RectangleShape res1366x768;
        res1366x768.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        res1366x768.setPosition(Vector2f(windowModeSizeX/2.5 + res1366x768.getSize().x*1.2*1, windowModeSizeY/10 + res1366x768.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 1366 && tempWindowModeSizeY == 768)
            res1366x768.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 1366 && tempWindowModeSizeY != 768)
            res1366x768.setFillColor(Color(113, 127, 128));
        res1366x768.setOutlineColor(Color(255,255,255));
        res1366x768.setOutlineThickness((windowModeSizeX*4)/1920);
        Text res1366x768Text;
        res1366x768Text.setFont(font);
        res1366x768Text.setString("1366x768");
        res1366x768Text.setCharacterSize((windowModeSizeX*60)/1920);
        res1366x768Text.setFillColor(Color(255,255,255));
        FloatRect res1366x768TextRect = res1366x768Text.getLocalBounds();
        res1366x768Text.setOrigin(res1366x768TextRect.left+res1366x768TextRect.width/2.0f,res1366x768TextRect.top+res1366x768TextRect.height/2.0f);
        res1366x768Text.setPosition(res1366x768.getPosition().x+res1366x768.getSize().x/2, res1366x768.getPosition().y+res1366x768.getSize().y/2);

        //Options res1024x576
        RectangleShape res1024x576;
        res1024x576.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        res1024x576.setPosition(Vector2f(windowModeSizeX/2.5 + res1024x576.getSize().x*1.2*2, windowModeSizeY/10 + res1024x576.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 1024 && tempWindowModeSizeY == 576)
            res1024x576.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 1024 && tempWindowModeSizeY != 576)
            res1024x576.setFillColor(Color(113, 127, 128));
        res1024x576.setOutlineColor(Color(255,255,255));
        res1024x576.setOutlineThickness((windowModeSizeX*4)/1920);
        Text res1024x576Text;
        res1024x576Text.setFont(font);
        res1024x576Text.setString("1024x576");
        res1024x576Text.setCharacterSize((windowModeSizeX*60)/1920);
        res1024x576Text.setFillColor(Color(255,255,255));
        FloatRect res1024x576TextRect = res1024x576Text.getLocalBounds();
        res1024x576Text.setOrigin(res1024x576TextRect.left+res1024x576TextRect.width/2.0f,res1024x576TextRect.top+res1024x576TextRect.height/2.0f);
        res1024x576Text.setPosition(res1024x576.getPosition().x+res1024x576.getSize().x/2, res1024x576.getPosition().y+res1024x576.getSize().y/2);

        //Options res800x450
        RectangleShape res800x450;
        res800x450.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        res800x450.setPosition(Vector2f(windowModeSizeX/2.5 + res800x450.getSize().x*1.2*3, windowModeSizeY/10 + res800x450.getSize().y*1.1*2));
        if(tempWindowModeSizeX == 800 && tempWindowModeSizeY == 450)
            res800x450.setFillColor(Color(96, 184, 84));
        else if(tempWindowModeSizeX != 800 && tempWindowModeSizeY != 450)
            res800x450.setFillColor(Color(113, 127, 128));
        res800x450.setOutlineColor(Color(255,255,255));
        res800x450.setOutlineThickness((windowModeSizeX*4)/1920);
        Text res800x450Text;
        res800x450Text.setFont(font);
        res800x450Text.setString("800x450");
        res800x450Text.setCharacterSize((windowModeSizeX*60)/1920);
        res800x450Text.setFillColor(Color(255,255,255));
        FloatRect res800x450TextRect = res800x450Text.getLocalBounds();
        res800x450Text.setOrigin(res800x450TextRect.left+res800x450TextRect.width/2.0f,res800x450TextRect.top+res800x450TextRect.height/2.0f);
        res800x450Text.setPosition(res800x450.getPosition().x+res800x450.getSize().x/2, res800x450.getPosition().y+res800x450.getSize().y/2);

        //Options music 0%
        RectangleShape musicVol0;
        musicVol0.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        musicVol0.setPosition(Vector2f(windowModeSizeX/2.5 + musicVol0.getSize().x*1.2*0, windowModeSizeY/10 + musicVol0.getSize().y*1.1*4));
        if(musicVolume == 0)
            musicVol0.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 0)
            musicVol0.setFillColor(Color(113, 127, 128));
        musicVol0.setOutlineColor(Color(255,255,255));
        musicVol0.setOutlineThickness((windowModeSizeX*4)/1920);
        Text musicVol0Text;
        musicVol0Text.setFont(font);
        musicVol0Text.setString("0%");
        musicVol0Text.setCharacterSize((windowModeSizeX*60)/1920);
        musicVol0Text.setFillColor(Color(255,255,255));
        FloatRect musicVol0TextRect = musicVol0Text.getLocalBounds();
        musicVol0Text.setOrigin(musicVol0TextRect.left+musicVol0TextRect.width/2.0f,musicVol0TextRect.top+musicVol0TextRect.height/2.0f);
        musicVol0Text.setPosition(musicVol0.getPosition().x+musicVol0.getSize().x/2, musicVol0.getPosition().y+musicVol0.getSize().y/2);

        //Options music 25%
        RectangleShape musicVol25;
        musicVol25.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        musicVol25.setPosition(Vector2f(windowModeSizeX/2.5 + musicVol25.getSize().x*1.2*1, windowModeSizeY/10 + musicVol25.getSize().y*1.1*4));
        if(musicVolume == 0.25)
            musicVol25.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 0.25)
            musicVol25.setFillColor(Color(113, 127, 128));
        musicVol25.setOutlineColor(Color(255,255,255));
        musicVol25.setOutlineThickness((windowModeSizeX*4)/1920);
        Text musicVol25Text;
        musicVol25Text.setFont(font);
        musicVol25Text.setString("25%");
        musicVol25Text.setCharacterSize((windowModeSizeX*60)/1920);
        musicVol25Text.setFillColor(Color(255,255,255));
        FloatRect musicVol25TextRect = musicVol25Text.getLocalBounds();
        musicVol25Text.setOrigin(musicVol25TextRect.left+musicVol25TextRect.width/2.0f,musicVol25TextRect.top+musicVol25TextRect.height/2.0f);
        musicVol25Text.setPosition(musicVol25.getPosition().x+musicVol25.getSize().x/2, musicVol25.getPosition().y+musicVol25.getSize().y/2);

        //Options music 50%
        RectangleShape musicVol50;
        musicVol50.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        musicVol50.setPosition(Vector2f(windowModeSizeX/2.5 + musicVol50.getSize().x*1.2*2, windowModeSizeY/10 + musicVol50.getSize().y*1.1*4));
        if(musicVolume == 0.50)
            musicVol50.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 0.50)
            musicVol50.setFillColor(Color(113, 127, 128));
        musicVol50.setOutlineColor(Color(255,255,255));
        musicVol50.setOutlineThickness((windowModeSizeX*4)/1920);
        Text musicVol50Text;
        musicVol50Text.setFont(font);
        musicVol50Text.setString("50%");
        musicVol50Text.setCharacterSize((windowModeSizeX*60)/1920);
        musicVol50Text.setFillColor(Color(255,255,255));
        FloatRect musicVol50TextRect = musicVol50Text.getLocalBounds();
        musicVol50Text.setOrigin(musicVol50TextRect.left+musicVol50TextRect.width/2.0f,musicVol50TextRect.top+musicVol50TextRect.height/2.0f);
        musicVol50Text.setPosition(musicVol50.getPosition().x+musicVol50.getSize().x/2, musicVol50.getPosition().y+musicVol50.getSize().y/2);

        //Options music 100%
        RectangleShape musicVol100;
        musicVol100.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        musicVol100.setPosition(Vector2f(windowModeSizeX/2.5 + musicVol100.getSize().x*1.2*3, windowModeSizeY/10 + musicVol100.getSize().y*1.1*4));
        if(musicVolume == 1)
            musicVol100.setFillColor(Color(96, 184, 84));
        else if(musicVolume != 1)
            musicVol100.setFillColor(Color(113, 127, 128));
        musicVol100.setOutlineColor(Color(255,255,255));
        musicVol100.setOutlineThickness((windowModeSizeX*4)/1920);
        Text musicVol100Text;
        musicVol100Text.setFont(font);
        musicVol100Text.setString("100%");
        musicVol100Text.setCharacterSize((windowModeSizeX*60)/1920);
        musicVol100Text.setFillColor(Color(255,255,255));
        FloatRect musicVol100TextRect = musicVol100Text.getLocalBounds();
        musicVol100Text.setOrigin(musicVol100TextRect.left+musicVol100TextRect.width/2.0f,musicVol100TextRect.top+musicVol100TextRect.height/2.0f);
        musicVol100Text.setPosition(musicVol100.getPosition().x+musicVol100.getSize().x/2, musicVol100.getPosition().y+musicVol100.getSize().y/2);

        //Options sfx 0%
        RectangleShape sfxVol0;
        sfxVol0.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        sfxVol0.setPosition(Vector2f(windowModeSizeX/2.5 + sfxVol0.getSize().x*1.2*0, windowModeSizeY/10 + sfxVol0.getSize().y*1.1*6));
        if(sfxVolume == 0)
            sfxVol0.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 0)
            sfxVol0.setFillColor(Color(113, 127, 128));
        sfxVol0.setOutlineColor(Color(255,255,255));
        sfxVol0.setOutlineThickness((windowModeSizeX*4)/1920);
        Text sfxVol0Text;
        sfxVol0Text.setFont(font);
        sfxVol0Text.setString("0%");
        sfxVol0Text.setCharacterSize((windowModeSizeX*60)/1920);
        sfxVol0Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol0TextRect = sfxVol0Text.getLocalBounds();
        sfxVol0Text.setOrigin(sfxVol0TextRect.left+sfxVol0TextRect.width/2.0f,sfxVol0TextRect.top+sfxVol0TextRect.height/2.0f);
        sfxVol0Text.setPosition(sfxVol0.getPosition().x+sfxVol0.getSize().x/2, sfxVol0.getPosition().y+sfxVol0.getSize().y/2);

        //Options sfx 25%
        RectangleShape sfxVol25;
        sfxVol25.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        sfxVol25.setPosition(Vector2f(windowModeSizeX/2.5 + sfxVol25.getSize().x*1.2*1, windowModeSizeY/10 + sfxVol25.getSize().y*1.1*6));
        if(sfxVolume == 0.25)
            sfxVol25.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 0.25)
            sfxVol25.setFillColor(Color(113, 127, 128));
        sfxVol25.setOutlineColor(Color(255,255,255));
        sfxVol25.setOutlineThickness((windowModeSizeX*4)/1920);
        Text sfxVol25Text;
        sfxVol25Text.setFont(font);
        sfxVol25Text.setString("25%");
        sfxVol25Text.setCharacterSize((windowModeSizeX*60)/1920);
        sfxVol25Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol25TextRect = sfxVol25Text.getLocalBounds();
        sfxVol25Text.setOrigin(sfxVol25TextRect.left+sfxVol25TextRect.width/2.0f,sfxVol25TextRect.top+sfxVol25TextRect.height/2.0f);
        sfxVol25Text.setPosition(sfxVol25.getPosition().x+sfxVol25.getSize().x/2, sfxVol25.getPosition().y+sfxVol25.getSize().y/2);

        //Options sfx 50%
        RectangleShape sfxVol50;
        sfxVol50.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        sfxVol50.setPosition(Vector2f(windowModeSizeX/2.5 + sfxVol50.getSize().x*1.2*2, windowModeSizeY/10 + sfxVol50.getSize().y*1.1*6));
        if(sfxVolume == 0.50)
            sfxVol50.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 0.50)
            sfxVol50.setFillColor(Color(113, 127, 128));
        sfxVol50.setOutlineColor(Color(255,255,255));
        sfxVol50.setOutlineThickness((windowModeSizeX*4)/1920);
        Text sfxVol50Text;
        sfxVol50Text.setFont(font);
        sfxVol50Text.setString("50%");
        sfxVol50Text.setCharacterSize((windowModeSizeX*60)/1920);
        sfxVol50Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol50TextRect = sfxVol50Text.getLocalBounds();
        sfxVol50Text.setOrigin(sfxVol50TextRect.left+sfxVol50TextRect.width/2.0f,sfxVol50TextRect.top+sfxVol50TextRect.height/2.0f);
        sfxVol50Text.setPosition(sfxVol50.getPosition().x+sfxVol50.getSize().x/2, sfxVol50.getPosition().y+sfxVol50.getSize().y/2);

        //Options sfx 100%
        RectangleShape sfxVol100;
        sfxVol100.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        sfxVol100.setPosition(Vector2f(windowModeSizeX/2.5 + sfxVol100.getSize().x*1.2*3, windowModeSizeY/10 + sfxVol100.getSize().y*1.1*6));
        if(sfxVolume == 1)
            sfxVol100.setFillColor(Color(96, 184, 84));
        else if(sfxVolume != 1)
            sfxVol100.setFillColor(Color(113, 127, 128));
        sfxVol100.setOutlineColor(Color(255,255,255));
        sfxVol100.setOutlineThickness((windowModeSizeX*4)/1920);
        Text sfxVol100Text;
        sfxVol100Text.setFont(font);
        sfxVol100Text.setString("100%");
        sfxVol100Text.setCharacterSize((windowModeSizeX*60)/1920);
        sfxVol100Text.setFillColor(Color(255,255,255));
        FloatRect sfxVol100TextRect = sfxVol100Text.getLocalBounds();
        sfxVol100Text.setOrigin(sfxVol100TextRect.left+sfxVol100TextRect.width/2.0f,sfxVol100TextRect.top+sfxVol100TextRect.height/2.0f);
        sfxVol100Text.setPosition(sfxVol100.getPosition().x+sfxVol100.getSize().x/2, sfxVol100.getPosition().y+sfxVol100.getSize().y/2);

        //Options Delay 0.1
        RectangleShape delay01;
        delay01.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        delay01.setPosition(Vector2f(windowModeSizeX/2.5 + delay01.getSize().x*1.2*0, windowModeSizeY/10 + delay01.getSize().y*1.1*8));
        if(levelUpdateDelay == (float)0.1)
            delay01.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 0.1)
            delay01.setFillColor(Color(113, 127, 128));
        delay01.setOutlineColor(Color(255,255,255));
        delay01.setOutlineThickness((windowModeSizeX*4)/1920);
        Text delay01Text;
        delay01Text.setFont(font);
        delay01Text.setString("0.1s");
        delay01Text.setCharacterSize((windowModeSizeX*60)/1920);
        delay01Text.setFillColor(Color(255,255,255));
        FloatRect delay01TextRect = delay01Text.getLocalBounds();
        delay01Text.setOrigin(delay01TextRect.left+delay01TextRect.width/2.0f,delay01TextRect.top+delay01TextRect.height/2.0f);
        delay01Text.setPosition(delay01.getPosition().x+delay01.getSize().x/2, delay01.getPosition().y+delay01.getSize().y/2);

        //Options Delay 0.5
        RectangleShape delay05;
        delay05.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        delay05.setPosition(Vector2f(windowModeSizeX/2.5 + delay05.getSize().x*1.2*1, windowModeSizeY/10 + delay05.getSize().y*1.1*8));
        if(levelUpdateDelay == 0.5)
            delay05.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 0.5)
            delay05.setFillColor(Color(113, 127, 128));
        delay05.setOutlineColor(Color(255,255,255));
        delay05.setOutlineThickness((windowModeSizeX*4)/1920);
        Text delay05Text;
        delay05Text.setFont(font);
        delay05Text.setString("0.5s");
        delay05Text.setCharacterSize((windowModeSizeX*60)/1920);
        delay05Text.setFillColor(Color(255,255,255));
        FloatRect delay05TextRect = delay05Text.getLocalBounds();
        delay05Text.setOrigin(delay05TextRect.left+delay05TextRect.width/2.0f,delay05TextRect.top+delay05TextRect.height/2.0f);
        delay05Text.setPosition(delay05.getPosition().x+delay05.getSize().x/2, delay05.getPosition().y+delay05.getSize().y/2);

        //Options Delay 10
        RectangleShape delay10;
        delay10.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        delay10.setPosition(Vector2f(windowModeSizeX/2.5 + delay10.getSize().x*1.2*2, windowModeSizeY/10 + delay10.getSize().y*1.1*8));
        if(levelUpdateDelay == 1)
            delay10.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 1)
            delay10.setFillColor(Color(113, 127, 128));
        delay10.setOutlineColor(Color(255,255,255));
        delay10.setOutlineThickness((windowModeSizeX*4)/1920);
        Text delay10Text;
        delay10Text.setFont(font);
        delay10Text.setString("1s");
        delay10Text.setCharacterSize((windowModeSizeX*60)/1920);
        delay10Text.setFillColor(Color(255,255,255));
        FloatRect delay10TextRect = delay10Text.getLocalBounds();
        delay10Text.setOrigin(delay10TextRect.left+delay10TextRect.width/2.0f,delay10TextRect.top+delay10TextRect.height/2.0f);
        delay10Text.setPosition(delay10.getPosition().x+delay10.getSize().x/2, delay10.getPosition().y+delay10.getSize().y/2);

        //Options Delay 20
        RectangleShape delay20;
        delay20.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        delay20.setPosition(Vector2f(windowModeSizeX/2.5 + delay20.getSize().x*1.2*3, windowModeSizeY/10 + delay20.getSize().y*1.1*8));
        if(levelUpdateDelay == 2)
            delay20.setFillColor(Color(96, 184, 84));
        else if(levelUpdateDelay != 2)
            delay20.setFillColor(Color(113, 127, 128));
        delay20.setOutlineColor(Color(255,255,255));
        delay20.setOutlineThickness((windowModeSizeX*4)/1920);
        Text delay20Text;
        delay20Text.setFont(font);
        delay20Text.setString("2s");
        delay20Text.setCharacterSize((windowModeSizeX*60)/1920);
        delay20Text.setFillColor(Color(255,255,255));
        FloatRect delay20TextRect = delay20Text.getLocalBounds();
        delay20Text.setOrigin(delay20TextRect.left+delay20TextRect.width/2.0f,delay20TextRect.top+delay20TextRect.height/2.0f);
        delay20Text.setPosition(delay20.getPosition().x+delay20.getSize().x/2, delay20.getPosition().y+delay20.getSize().y/2);

        //Options textureDefault
        RectangleShape txtDefault;
        txtDefault.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        txtDefault.setPosition(Vector2f(windowModeSizeX/2.5 + txtDefault.getSize().x*1.2*0, windowModeSizeY/10 + txtDefault.getSize().y*1.1*10));
        if(texturePath == "default")
            txtDefault.setFillColor(Color(96, 184, 84));
        else if(texturePath != "default")
            txtDefault.setFillColor(Color(113, 127, 128));
        txtDefault.setOutlineColor(Color(255,255,255));
        txtDefault.setOutlineThickness((windowModeSizeX*4)/1920);
        Text txtDefaultText;
        txtDefaultText.setFont(font);
        txtDefaultText.setString("Default");
        txtDefaultText.setCharacterSize((windowModeSizeX*60)/1920);
        txtDefaultText.setFillColor(Color(255,255,255));
        FloatRect txtDefaultTextRect = txtDefaultText.getLocalBounds();
        txtDefaultText.setOrigin(txtDefaultTextRect.left+txtDefaultTextRect.width/2.0f,txtDefaultTextRect.top+txtDefaultTextRect.height/2.0f);
        txtDefaultText.setPosition(txtDefault.getPosition().x+txtDefault.getSize().x/2, txtDefault.getPosition().y+txtDefault.getSize().y/2);

        //Options textureDark
        RectangleShape txtDark;
        txtDark.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        txtDark.setPosition(Vector2f(windowModeSizeX/2.5 + txtDark.getSize().x*1.2*1, windowModeSizeY/10 + txtDark.getSize().y*1.1*10));
        if(texturePath == "dark")
            txtDark.setFillColor(Color(96, 184, 84));
        else if(texturePath != "dark")
            txtDark.setFillColor(Color(113, 127, 128));
        txtDark.setOutlineColor(Color(255,255,255));
        txtDark.setOutlineThickness((windowModeSizeX*4)/1920);
        Text txtDarkText;
        txtDarkText.setFont(font);
        txtDarkText.setString("Dark");
        txtDarkText.setCharacterSize((windowModeSizeX*60)/1920);
        txtDarkText.setFillColor(Color(255,255,255));
        FloatRect txtDarkTextRect = txtDarkText.getLocalBounds();
        txtDarkText.setOrigin(txtDarkTextRect.left+txtDarkTextRect.width/2.0f,txtDarkTextRect.top+txtDarkTextRect.height/2.0f);
        txtDarkText.setPosition(txtDark.getPosition().x+txtDark.getSize().x/2, txtDark.getPosition().y+txtDark.getSize().y/2);

        //Options textureClassic
        RectangleShape txtClassic;
        txtClassic.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        txtClassic.setPosition(Vector2f(windowModeSizeX/2.5 + txtClassic.getSize().x*1.2*2, windowModeSizeY/10 + txtClassic.getSize().y*1.1*10));
        if(texturePath == "classic")
            txtClassic.setFillColor(Color(96, 184, 84));
        else if(texturePath != "classic")
            txtClassic.setFillColor(Color(113, 127, 128));
        txtClassic.setOutlineColor(Color(255,255,255));
        txtClassic.setOutlineThickness((windowModeSizeX*4)/1920);
        Text txtClassicText;
        txtClassicText.setFont(font);
        txtClassicText.setString("Classic");
        txtClassicText.setCharacterSize((windowModeSizeX*60)/1920);
        txtClassicText.setFillColor(Color(255,255,255));
        FloatRect txtClassicTextRect = txtClassicText.getLocalBounds();
        txtClassicText.setOrigin(txtClassicTextRect.left+txtClassicTextRect.width/2.0f,txtClassicTextRect.top+txtClassicTextRect.height/2.0f);
        txtClassicText.setPosition(txtClassic.getPosition().x+txtClassic.getSize().x/2, txtClassic.getPosition().y+txtClassic.getSize().y/2);

        //Options textureCustom
        RectangleShape txtCustom;
        txtCustom.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        txtCustom.setPosition(Vector2f(windowModeSizeX/2.5 + txtCustom.getSize().x*1.2*3, windowModeSizeY/10 + txtCustom.getSize().y*1.1*10));
        if(texturePath == "custom")
            txtCustom.setFillColor(Color(96, 184, 84));
        else if(texturePath != "custom")
            txtCustom.setFillColor(Color(113, 127, 128));
        txtCustom.setOutlineColor(Color(255,255,255));
        txtCustom.setOutlineThickness((windowModeSizeX*4)/1920);
        Text txtCustomText;
        txtCustomText.setFont(font);
        txtCustomText.setString("Custom");
        txtCustomText.setCharacterSize((windowModeSizeX*60)/1920);
        txtCustomText.setFillColor(Color(255,255,255));
        FloatRect txtCustomTextRect = txtCustomText.getLocalBounds();
        txtCustomText.setOrigin(txtCustomTextRect.left+txtCustomTextRect.width/2.0f,txtCustomTextRect.top+txtCustomTextRect.height/2.0f);
        txtCustomText.setPosition(txtCustom.getPosition().x+txtCustom.getSize().x/2, txtCustom.getPosition().y+txtCustom.getSize().y/2);


        //Exit Button:
        RectangleShape exitButton;
        exitButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        exitButton.setOrigin(Vector2f(exitButton.getLocalBounds().width/2, exitButton.getGlobalBounds().height/2));
        exitButton.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2 + exitButton.getSize().y*1.2*3));
        exitButton.setFillColor(Color(222, 55, 55));
        exitButton.setOutlineColor(Color(255,255,255));
        exitButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text exitButtonText;
        exitButtonText.setFont(font);
        exitButtonText.setString("Exit");
        exitButtonText.setCharacterSize((windowModeSizeX*60)/1920);
        exitButtonText.setFillColor(Color(255,255,255));
        FloatRect exitButtonTextRect = exitButtonText.getLocalBounds();
        exitButtonText.setOrigin(exitButtonTextRect.left+exitButtonTextRect.width/2.0f,exitButtonTextRect.top+exitButtonTextRect.height/2.0f);
        exitButtonText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2 + exitButton.getSize().y*1.2*3));

        //ExitConfirmationMessage:
        RectangleShape exitConfirmationMessage;
        exitConfirmationMessage.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        exitConfirmationMessage.setOrigin(Vector2f(exitConfirmationMessage.getLocalBounds().width/2, exitConfirmationMessage.getGlobalBounds().height/2));
        exitConfirmationMessage.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));
        exitConfirmationMessage.setFillColor(Color(1,14,36,0));
        exitConfirmationMessage.setOutlineColor(Color(1,14,36,0));
        exitConfirmationMessage.setOutlineThickness((windowModeSizeX*4)/1920);
        Text exitConfirmationText;
        exitConfirmationText.setFont(font);
        exitConfirmationText.setString("Are you sure you want to exit?");
        exitConfirmationText.setCharacterSize((windowModeSizeX*60)/1920);
        exitConfirmationText.setFillColor(Color(255,255,255));
        FloatRect exitConfirmationTextRect = exitConfirmationText.getLocalBounds();
        exitConfirmationText.setOrigin(exitConfirmationTextRect.left+exitConfirmationTextRect.width/2.0f,exitConfirmationTextRect.top+exitConfirmationTextRect.height/2.0f);
        exitConfirmationText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));
        exitConfirmationText.setOutlineColor(Color(0,0,0));
        exitConfirmationText.setOutlineThickness((windowModeSizeX*8)/1920);


        //ExitConfirmationMessageYES:
        RectangleShape exitYesButton;
        exitYesButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        exitYesButton.setOrigin(Vector2f(exitYesButton.getLocalBounds().width/2, exitYesButton.getGlobalBounds().height/2));
        exitYesButton.setPosition(Vector2f(windowModeSizeX/2 - exitYesButton.getSize().x, windowModeSizeY/2 + exitYesButton.getSize().y*1.2));
        exitYesButton.setFillColor(Color(222, 55, 55));
        exitYesButton.setOutlineColor(Color(255,255,255));
        exitYesButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text exitYesButtonText;
        exitYesButtonText.setFont(font);
        exitYesButtonText.setString("Exit");
        exitYesButtonText.setCharacterSize((windowModeSizeX*60)/1920);
        exitYesButtonText.setFillColor(Color(255,255,255));
        FloatRect exitYesButtonTextRect = exitYesButtonText.getLocalBounds();
        exitYesButtonText.setOrigin(exitYesButtonTextRect.left+exitYesButtonTextRect.width/2.0f,exitYesButtonTextRect.top+exitYesButtonTextRect.height/2.0f);
        exitYesButtonText.setPosition(Vector2f(windowModeSizeX/2 - exitYesButton.getSize().x, windowModeSizeY/2 + exitYesButton.getSize().y*1.2));

        //ExitConfirmationMessageNo:
        RectangleShape exitNoButton;
        exitNoButton.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        exitNoButton.setOrigin(Vector2f(exitNoButton.getLocalBounds().width/2, exitNoButton.getGlobalBounds().height/2));
        exitNoButton.setPosition(Vector2f(windowModeSizeX/2 + exitNoButton.getSize().x, windowModeSizeY/2 + exitNoButton.getSize().y*1.2));
        exitNoButton.setFillColor(Color(96, 184, 84));
        exitNoButton.setOutlineColor(Color(255,255,255));
        exitNoButton.setOutlineThickness((windowModeSizeX*4)/1920);
        Text exitNoButtonText;
        exitNoButtonText.setFont(font);
        exitNoButtonText.setString("Stay");
        exitNoButtonText.setCharacterSize((windowModeSizeX*60)/1920);
        exitNoButtonText.setFillColor(Color(255,255,255));
        FloatRect exitNoButtonTextRect = exitNoButtonText.getLocalBounds();
        exitNoButtonText.setOrigin(exitNoButtonTextRect.left+exitNoButtonTextRect.width/2.0f,exitNoButtonTextRect.top+exitNoButtonTextRect.height/2.0f);
        exitNoButtonText.setPosition(Vector2f(windowModeSizeX/2 + exitNoButton.getSize().x, windowModeSizeY/2 + exitNoButton.getSize().y*1.2));

        //Size option:
        RectangleShape sizeMsg;
        sizeMsg.setSize(Vector2f(windowModeSizeX/6, windowModeSizeY / 10));
        sizeMsg.setOrigin(Vector2f(sizeMsg.getLocalBounds().width/2, sizeMsg.getGlobalBounds().height/2));
        sizeMsg.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));
        sizeMsg.setFillColor(Color(1,14,36,0));
        sizeMsg.setOutlineColor(Color(1,14,36,0));
        sizeMsg.setOutlineThickness((windowModeSizeX*4)/1920);
        Text sizeMsgText;
        sizeMsgText.setFont(font);
        sizeMsgText.setString("Select world size:");
        sizeMsgText.setCharacterSize((windowModeSizeX*60)/1920);
        sizeMsgText.setFillColor(Color(255,255,255));
        FloatRect sizeMsgTextRect = sizeMsgText.getLocalBounds();
        sizeMsgText.setOrigin(sizeMsgTextRect.left+sizeMsgTextRect.width/2.0f,sizeMsgTextRect.top+sizeMsgTextRect.height/2.0f);
        sizeMsgText.setPosition(Vector2f(windowModeSizeX/2, windowModeSizeY/2));
        sizeMsgText.setOutlineColor(Color(0,0,0));
        sizeMsgText.setOutlineThickness((windowModeSizeX*8)/1920);

        //Optionssize 25
        RectangleShape size25;
        size25.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        size25.setOrigin(Vector2f(size25.getSize().x/2,size25.getSize().y/2));
        size25.setPosition(Vector2f(windowModeSizeX/2 - size25.getSize().x*2, windowModeSizeY/2 + sizeMsg.getSize().y*1.2));
        size25.setFillColor(Color(113, 127, 128));
        size25.setOutlineColor(Color(255,255,255));
        size25.setOutlineThickness((windowModeSizeX*4)/1920);
        Text size25Text;
        size25Text.setFont(font);
        size25Text.setString("25x25");
        size25Text.setCharacterSize((windowModeSizeX*60)/1920);
        size25Text.setFillColor(Color(255,255,255));
        FloatRect size25TextRect = size25Text.getLocalBounds();
        size25Text.setOrigin(size25TextRect.left+size25TextRect.width/2.0f,size25TextRect.top+size25TextRect.height/2.0f);
        size25Text.setPosition(size25.getPosition().x, size25.getPosition().y);

        //Optionssize 50
        RectangleShape size50;
        size50.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        size50.setOrigin(Vector2f(size50.getSize().x/2,size50.getSize().y/2));
        size50.setPosition(Vector2f(windowModeSizeX/2 - size50.getSize().x*0.7, windowModeSizeY/2 + sizeMsg.getSize().y*1.2));
        size50.setFillColor(Color(113, 127, 128));
        size50.setOutlineColor(Color(255,255,255));
        size50.setOutlineThickness((windowModeSizeX*4)/1920);
        Text size50Text;
        size50Text.setFont(font);
        size50Text.setString("50x50");
        size50Text.setCharacterSize((windowModeSizeX*60)/1920);
        size50Text.setFillColor(Color(255,255,255));
        FloatRect size50TextRect = size50Text.getLocalBounds();
        size50Text.setOrigin(size50TextRect.left+size50TextRect.width/2.0f,size50TextRect.top+size50TextRect.height/2.0f);
        size50Text.setPosition(size50.getPosition().x, size50.getPosition().y);

        //Optionssize 75
        RectangleShape size75;
        size75.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        size75.setOrigin(Vector2f(size75.getSize().x/2,size75.getSize().y/2));
        size75.setPosition(Vector2f(windowModeSizeX/2 + size75.getSize().x*0.7, windowModeSizeY/2 + sizeMsg.getSize().y*1.2));
        size75.setFillColor(Color(113, 127, 128));
        size75.setOutlineColor(Color(255,255,255));
        size75.setOutlineThickness((windowModeSizeX*4)/1920);
        Text size75Text;
        size75Text.setFont(font);
        size75Text.setString("75x75");
        size75Text.setCharacterSize((windowModeSizeX*60)/1920);
        size75Text.setFillColor(Color(255,255,255));
        FloatRect size75TextRect = size75Text.getLocalBounds();
        size75Text.setOrigin(size75TextRect.left+size75TextRect.width/2.0f,size75TextRect.top+size75TextRect.height/2.0f);
        size75Text.setPosition(size75.getPosition().x, size75.getPosition().y);

        //Optionssize 100
        RectangleShape size100;
        size100.setSize(Vector2f(windowModeSizeX/8, windowModeSizeY / 20));
        size100.setOrigin(Vector2f(size100.getSize().x/2,size100.getSize().y/2));
        size100.setPosition(Vector2f(windowModeSizeX/2 + size100.getSize().x*2, windowModeSizeY/2 + sizeMsg.getSize().y*1.2));
        size100.setFillColor(Color(113, 127, 128));
        size100.setOutlineColor(Color(255,255,255));
        size100.setOutlineThickness((windowModeSizeX*4)/1920);
        Text size100Text;
        size100Text.setFont(font);
        size100Text.setString("100x100");
        size100Text.setCharacterSize((windowModeSizeX*60)/1920);
        size100Text.setFillColor(Color(255,255,255));
        FloatRect size100TextRect = size100Text.getLocalBounds();
        size100Text.setOrigin(size100TextRect.left+size100TextRect.width/2.0f,size100TextRect.top+size100TextRect.height/2.0f);
        size100Text.setPosition(size100.getPosition().x, size100.getPosition().y);

        while(menu.isOpen())
        {
            //std::cout << levelUpdateDelay << std::endl;

            while(menu.pollEvent(menuEvent))
            {
                if(menuEvent.type == Event::Closed)
                {
                    menu.close();
                    return -1;
                }
            }

            //check enter/escape presses:
            if(Keyboard::isKeyPressed(Keyboard::Enter) && inputBuffer.restart().asMilliseconds() >= inputDelay)
            {
                if(menuID == -1)
                {
                    menu.close();
                    return -1;
                }
            }
            if(Keyboard::isKeyPressed(Keyboard::Escape) && inputBuffer.restart().asMilliseconds() >= inputDelay)
            {
                if(menuID == 0)
                    menuID = -1;
                else if(menuID == 1)
                    menuID = 0;
                else if(menuID == 2)
                    menuID = 0;
                else if(menuID == 3)
                    menuID = 0;
                else if(menuID == 5)
                    menuID = 3;
            }

            //CheckButtonPressed:
            Vector2i mP = Mouse::getPosition(menu);
            if(menu.hasFocus())
            switch(menuID)
            {
            case -1:
                if(Mouse::isButtonPressed(Mouse::Left) && exitYesButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menu.close();
                    return -1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && exitNoButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;
                }
                break;

            case 0:
                if(Mouse::isButtonPressed(Mouse::Left) && exitButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = -1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && guideButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && optionsButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && playButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 3;
                }
                break;

            case 1:
                if(Mouse::isButtonPressed(Mouse::Left) && guideBackButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;
                }
                break;

            case 2:
                if(Mouse::isButtonPressed(Mouse::Left) && optionsBackButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;

                    tempFullscreen = Fullscreen;
                    tempWindowModeSizeX = windowModeSizeX;
                    tempWindowModeSizeY = windowModeSizeY;
                    tempMusicVolume = musicVolume;
                    tempSfxVolume = sfxVolume;
                    tempLevelUpdateDelay = levelUpdateDelay;
                    tempTexturePath = texturePath;

                    if(Fullscreen == false)
                    {
                        opFullscreenOff.setFillColor(Color(96, 184, 84));
                        opFullscreenOn.setFillColor(Color(113, 127, 128));
                    }
                    if(Fullscreen == true)
                    {
                        opFullscreenOff.setFillColor(Color(113, 127, 128));
                        opFullscreenOn.setFillColor(Color(96, 184, 84));
                    }

                    //Resolutions:
                    if(tempWindowModeSizeX == 1600 && tempWindowModeSizeY == 900)
                    {
                        res1600x900.setFillColor(Color(96, 184, 84));
                        res1366x768.setFillColor(Color(113, 127, 128));
                        res1024x576.setFillColor(Color(113, 127, 128));
                        res800x450.setFillColor(Color(113, 127, 128));
                    }
                    if(tempWindowModeSizeX == 1366 && tempWindowModeSizeY == 768)
                    {
                        res1600x900.setFillColor(Color(113, 127, 128));
                        res1366x768.setFillColor(Color(96, 184, 84));
                        res1024x576.setFillColor(Color(113, 127, 128));
                        res800x450.setFillColor(Color(113, 127, 128));
                    }
                    if(tempWindowModeSizeX == 1024 && tempWindowModeSizeY == 576)
                    {
                        res1600x900.setFillColor(Color(113, 127, 128));
                        res1366x768.setFillColor(Color(113, 127, 128));
                        res1024x576.setFillColor(Color(96, 184, 84));
                        res800x450.setFillColor(Color(113, 127, 128));
                    }
                    if(tempWindowModeSizeX == 800 && tempWindowModeSizeY == 450)
                    {
                        res1600x900.setFillColor(Color(113, 127, 128));
                        res1366x768.setFillColor(Color(113, 127, 128));
                        res1024x576.setFillColor(Color(113, 127, 128));
                        res800x450.setFillColor(Color(96, 184, 84));
                    }


                    //Music:
                    if(tempMusicVolume == 0)
                    {
                        musicVol0.setFillColor(Color(96, 184, 84));
                        musicVol25.setFillColor(Color(113, 127, 128));
                        musicVol50.setFillColor(Color(113, 127, 128));
                        musicVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempMusicVolume == 0.25)
                    {
                        musicVol0.setFillColor(Color(113, 127, 128));
                        musicVol25.setFillColor(Color(96, 184, 84));
                        musicVol50.setFillColor(Color(113, 127, 128));
                        musicVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempMusicVolume == 0.50)
                    {
                        musicVol0.setFillColor(Color(113, 127, 128));
                        musicVol25.setFillColor(Color(113, 127, 128));
                        musicVol50.setFillColor(Color(96, 184, 84));
                        musicVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempMusicVolume == 1)
                    {
                        musicVol0.setFillColor(Color(113, 127, 128));
                        musicVol25.setFillColor(Color(113, 127, 128));
                        musicVol50.setFillColor(Color(113, 127, 128));
                        musicVol100.setFillColor(Color(96, 184, 84));
                    }

                    //sfx:
                    if(tempSfxVolume == 0)
                    {
                        sfxVol0.setFillColor(Color(96, 184, 84));
                        sfxVol25.setFillColor(Color(113, 127, 128));
                        sfxVol50.setFillColor(Color(113, 127, 128));
                        sfxVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempSfxVolume == 0.25)
                    {
                        sfxVol0.setFillColor(Color(113, 127, 128));
                        sfxVol25.setFillColor(Color(96, 184, 84));
                        sfxVol50.setFillColor(Color(113, 127, 128));
                        sfxVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempSfxVolume == 0.50)
                    {
                        sfxVol0.setFillColor(Color(113, 127, 128));
                        sfxVol25.setFillColor(Color(113, 127, 128));
                        sfxVol50.setFillColor(Color(96, 184, 84));
                        sfxVol100.setFillColor(Color(113, 127, 128));
                    }
                    if(tempSfxVolume == 1)
                    {
                        sfxVol0.setFillColor(Color(113, 127, 128));
                        sfxVol25.setFillColor(Color(113, 127, 128));
                        sfxVol50.setFillColor(Color(113, 127, 128));
                        sfxVol100.setFillColor(Color(96, 184, 84));
                    }

                    //LevelUpdate delay:
                    if(tempLevelUpdateDelay == (float)0.1)
                    {
                        delay01.setFillColor(Color(96, 184, 84));
                        delay05.setFillColor(Color(113, 127, 128));
                        delay10.setFillColor(Color(113, 127, 128));
                        delay20.setFillColor(Color(113, 127, 128));
                    }
                    if(tempLevelUpdateDelay == 0.5)
                    {
                        delay01.setFillColor(Color(113, 127, 128));
                        delay05.setFillColor(Color(96, 184, 84));
                        delay10.setFillColor(Color(113, 127, 128));
                        delay20.setFillColor(Color(113, 127, 128));
                    }
                    if(tempLevelUpdateDelay == 1)
                    {
                        delay01.setFillColor(Color(113, 127, 128));
                        delay05.setFillColor(Color(113, 127, 128));
                        delay10.setFillColor(Color(96, 184, 84));
                        delay20.setFillColor(Color(113, 127, 128));
                    }
                    if(tempLevelUpdateDelay == 2)
                    {
                        delay01.setFillColor(Color(113, 127, 128));
                        delay05.setFillColor(Color(113, 127, 128));
                        delay10.setFillColor(Color(113, 127, 128));
                        delay20.setFillColor(Color(96, 184, 84));
                    }

                    //Texture path:
                    if(tempTexturePath == "default")
                    {
                        txtDefault.setFillColor(Color(96, 184, 84));
                        txtDark.setFillColor(Color(113, 127, 128));
                        txtClassic.setFillColor(Color(113, 127, 128));
                        txtCustom.setFillColor(Color(113, 127, 128));
                    }
                    if(tempTexturePath == "dark")
                    {
                        txtDefault.setFillColor(Color(113, 127, 128));
                        txtDark.setFillColor(Color(96, 184, 84));
                        txtClassic.setFillColor(Color(113, 127, 128));
                        txtCustom.setFillColor(Color(113, 127, 128));
                    }
                    if(tempTexturePath == "classic")
                    {
                        txtDefault.setFillColor(Color(113, 127, 128));
                        txtDark.setFillColor(Color(113, 127, 128));
                        txtClassic.setFillColor(Color(96, 184, 84));
                        txtCustom.setFillColor(Color(113, 127, 128));
                    }
                    if(tempTexturePath == "custom")
                    {
                        txtDefault.setFillColor(Color(113, 127, 128));
                        txtDark.setFillColor(Color(113, 127, 128));
                        txtClassic.setFillColor(Color(113, 127, 128));
                        txtCustom.setFillColor(Color(96, 184, 84));
                    }

                }
                if(Mouse::isButtonPressed(Mouse::Left) && optionsApplyButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    Fullscreen = tempFullscreen;
                    windowModeSizeX = tempWindowModeSizeX;
                    windowModeSizeY = tempWindowModeSizeY;
                    musicVolume = tempMusicVolume;
                    sfxVolume = tempSfxVolume;
                    levelUpdateDelay = tempLevelUpdateDelay;
                    texturePath = tempTexturePath;

                    saveConfig();
                    menu.close();
                    menuID = 2;
                    goto Start;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && opFullscreenOn.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    opFullscreenOn.setFillColor(Color(96, 184, 84));
                    opFullscreenOff.setFillColor(Color(113, 127, 128));
                    tempFullscreen = true;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && opFullscreenOff.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    opFullscreenOn.setFillColor(Color(113, 127, 128));
                    opFullscreenOff.setFillColor(Color(96, 184, 84));
                    tempFullscreen = false;
                }
                //Resolution:
                if(Mouse::isButtonPressed(Mouse::Left) && res1600x900.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(96, 184, 84));
                    res1366x768.setFillColor(Color(113, 127, 128));
                    res1024x576.setFillColor(Color(113, 127, 128));
                    res800x450.setFillColor(Color(113, 127, 128));
                    tempWindowModeSizeX = 1600;
                    tempWindowModeSizeY = 900;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && res1366x768.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(113, 127, 128));
                    res1366x768.setFillColor(Color(96, 184, 84));
                    res1024x576.setFillColor(Color(113, 127, 128));
                    res800x450.setFillColor(Color(113, 127, 128));
                    tempWindowModeSizeX = 1366;
                    tempWindowModeSizeY = 768;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && res1024x576.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(113, 127, 128));
                    res1366x768.setFillColor(Color(113, 127, 128));
                    res1024x576.setFillColor(Color(96, 184, 84));
                    res800x450.setFillColor(Color(113, 127, 128));
                    tempWindowModeSizeX = 1024;
                    tempWindowModeSizeY = 576;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && res800x450.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    res1600x900.setFillColor(Color(113, 127, 128));
                    res1366x768.setFillColor(Color(113, 127, 128));
                    res1024x576.setFillColor(Color(113, 127, 128));
                    res800x450.setFillColor(Color(96, 184, 84));
                    tempWindowModeSizeX = 800;
                    tempWindowModeSizeY = 450;
                }
                //Music:
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol0.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(96, 184, 84));
                    musicVol25.setFillColor(Color(113, 127, 128));
                    musicVol50.setFillColor(Color(113, 127, 128));
                    musicVol100.setFillColor(Color(113, 127, 128));
                    tempMusicVolume = 0;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol25.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(113, 127, 128));
                    musicVol25.setFillColor(Color(96, 184, 84));
                    musicVol50.setFillColor(Color(113, 127, 128));
                    musicVol100.setFillColor(Color(113, 127, 128));
                    tempMusicVolume = 0.25;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol50.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(113, 127, 128));
                    musicVol25.setFillColor(Color(113, 127, 128));
                    musicVol50.setFillColor(Color(96, 184, 84));
                    musicVol100.setFillColor(Color(113, 127, 128));
                    tempMusicVolume = 0.50;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && musicVol100.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    musicVol0.setFillColor(Color(113, 127, 128));
                    musicVol25.setFillColor(Color(113, 127, 128));
                    musicVol50.setFillColor(Color(113, 127, 128));
                    musicVol100.setFillColor(Color(96, 184, 84));
                    tempMusicVolume = 1;
                }
                //SFX:
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol0.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(96, 184, 84));
                    sfxVol25.setFillColor(Color(113, 127, 128));
                    sfxVol50.setFillColor(Color(113, 127, 128));
                    sfxVol100.setFillColor(Color(113, 127, 128));
                    tempSfxVolume = 0;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol25.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(113, 127, 128));
                    sfxVol25.setFillColor(Color(96, 184, 84));
                    sfxVol50.setFillColor(Color(113, 127, 128));
                    sfxVol100.setFillColor(Color(113, 127, 128));
                    tempSfxVolume = 0.25;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol50.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(113, 127, 128));
                    sfxVol25.setFillColor(Color(113, 127, 128));
                    sfxVol50.setFillColor(Color(96, 184, 84));
                    sfxVol100.setFillColor(Color(113, 127, 128));
                    tempSfxVolume = 0.50;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && sfxVol100.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    sfxVol0.setFillColor(Color(113, 127, 128));
                    sfxVol25.setFillColor(Color(113, 127, 128));
                    sfxVol50.setFillColor(Color(113, 127, 128));
                    sfxVol100.setFillColor(Color(96, 184, 84));
                    tempSfxVolume = 1;
                }
                //LevelUpdate delay:
                if(Mouse::isButtonPressed(Mouse::Left) && delay01.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(96, 184, 84));
                    delay05.setFillColor(Color(113, 127, 128));
                    delay10.setFillColor(Color(113, 127, 128));
                    delay20.setFillColor(Color(113, 127, 128));
                    tempLevelUpdateDelay = (float)0.1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && delay05.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(113, 127, 128));
                    delay05.setFillColor(Color(96, 184, 84));
                    delay10.setFillColor(Color(113, 127, 128));
                    delay20.setFillColor(Color(113, 127, 128));
                    tempLevelUpdateDelay = 0.5;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && delay10.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(113, 127, 128));
                    delay05.setFillColor(Color(113, 127, 128));
                    delay10.setFillColor(Color(96, 184, 84));
                    delay20.setFillColor(Color(113, 127, 128));
                    tempLevelUpdateDelay = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && delay20.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    delay01.setFillColor(Color(113, 127, 128));
                    delay05.setFillColor(Color(113, 127, 128));
                    delay10.setFillColor(Color(113, 127, 128));
                    delay20.setFillColor(Color(96, 184, 84));
                    tempLevelUpdateDelay = 2;
                }

                //TexturePath :
                if(Mouse::isButtonPressed(Mouse::Left) && txtDefault.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(96, 184, 84));
                    txtDark.setFillColor(Color(113, 127, 128));
                    txtClassic.setFillColor(Color(113, 127, 128));
                    txtCustom.setFillColor(Color(113, 127, 128));
                    tempTexturePath = "default";
                }
                if(Mouse::isButtonPressed(Mouse::Left) && txtDark.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(113, 127, 128));
                    txtDark.setFillColor(Color(96, 184, 84));
                    txtClassic.setFillColor(Color(113, 127, 128));
                    txtCustom.setFillColor(Color(113, 127, 128));
                    tempTexturePath = "dark";
                }
                if(Mouse::isButtonPressed(Mouse::Left) && txtClassic.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(113, 127, 128));
                    txtDark.setFillColor(Color(113, 127, 128));
                    txtClassic.setFillColor(Color(96, 184, 84));
                    txtCustom.setFillColor(Color(113, 127, 128));
                    tempTexturePath = "classic";
                }
                if(Mouse::isButtonPressed(Mouse::Left) && txtCustom.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    txtDefault.setFillColor(Color(113, 127, 128));
                    txtDark.setFillColor(Color(113, 127, 128));
                    txtClassic.setFillColor(Color(113, 127, 128));
                    txtCustom.setFillColor(Color(96, 184, 84));
                    tempTexturePath = "custom";
                }
                break;

            case 3:
                if(Mouse::isButtonPressed(Mouse::Left) && playBackButton.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 0;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot1.getGlobalBounds().contains(mP.x, mP.y) && is_slot1 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot2.getGlobalBounds().contains(mP.x, mP.y) && is_slot2 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot3.getGlobalBounds().contains(mP.x, mP.y) && is_slot3 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot4.getGlobalBounds().contains(mP.x, mP.y) && is_slot4 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 4;
                    saveSlot = 4;
                }

                //Delete/Create
                if(Mouse::isButtonPressed(Mouse::Left) && slot1DC.getGlobalBounds().contains(mP.x, mP.y) && is_slot1 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot2DC.getGlobalBounds().contains(mP.x, mP.y) && is_slot2 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot3DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot3 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot4DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot4 == false && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 6;
                    saveSlot = 4;
                }

                //Delete
                if(Mouse::isButtonPressed(Mouse::Left) && slot1DC.getGlobalBounds().contains(mP.x, mP.y) && is_slot1 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 1;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot2DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot2 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 2;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot3DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot3 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && slot4DC.getGlobalBounds().contains(mP.x, mP.y)  && is_slot4 == true && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    menuID = 5;
                    saveSlot = 4;
                }
                break;

            case 4:
                if(saveSlot == 1)
                {
                    Sleep(2000);
                    menu.close();
                    return 1;
                }
                if(saveSlot == 2)
                {
                    Sleep(2000);
                    menu.close();
                    return 2;
                }
                if(saveSlot == 3)
                {
                    Sleep(2000);
                    menu.close();
                    return 3;
                }
                if(saveSlot == 4)
                {
                    Sleep(2000);
                    menu.close();
                    return 4;
                }
                break;

            case 5:
                if(Mouse::isButtonPressed(Mouse::Left) && deleteMsgNo.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    saveSlot = -1;
                    menuID = 3;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && deleteMsgYes.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    if(saveSlot == 1)
                    {
                        is_slot1 = false;
                        deleteMap("slot1.map");
                    }
                    if(saveSlot == 2)
                    {
                        is_slot2 = false;
                        deleteMap("slot2.map");
                    }
                    if(saveSlot == 3)
                    {
                        is_slot3 = false;
                        deleteMap("slot3.map");
                    }
                    if(saveSlot == 4)
                    {
                        is_slot4 = false;
                        deleteMap("slot4.map");
                    }

                    menuID = 3;
                    goto Start;
                }
                break;

                case 6:
                if(Mouse::isButtonPressed(Mouse::Left) && size25.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 25;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && size50.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 50;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && size75.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 75;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                if(Mouse::isButtonPressed(Mouse::Left) && size100.getGlobalBounds().contains(mP.x, mP.y) && inputBuffer.restart().asMilliseconds() >= inputDelay)
                {
                    mapSize = 100;
                    if(saveSlot == 1)
                        createMap("slot1.map");
                    if(saveSlot == 2)
                        createMap("slot2.map");
                    if(saveSlot == 3)
                        createMap("slot3.map");
                    if(saveSlot == 4)
                        createMap("slot4.map");
                    menuID  = 4;
                }
                break;
            }

            //Drawing:
            menu.clear(Color(1,14,36));

            switch(menuID)
            {
            case -1:
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(exitConfirmationMessage);
                menu.draw(exitConfirmationText);

                menu.draw(exitYesButton);
                menu.draw(exitYesButtonText);

                menu.draw(exitNoButton);
                menu.draw(exitNoButtonText);
                break;
            case 0:
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(logoText);
                menu.draw(infoText);

                menu.draw(playButton);
                menu.draw(playButtonText);

                menu.draw(guideButton);
                menu.draw(guideButtonText);

                menu.draw(optionsButton);
                menu.draw(optionsButtonText);

                menu.draw(exitButton);
                menu.draw(exitButtonText);
                break;

            case 1:
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(guideMessageText);

                menu.draw(guideBackButton);
                menu.draw(guideBackText);
                break;

            case 2:
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(optionsMessage);

                menu.draw(opFullscreenOn);
                menu.draw(opFullscreenOnText);
                menu.draw(opFullscreenOff);
                menu.draw(opFullscreenOffText);

                menu.draw(res1600x900);
                menu.draw(res1600x900Text);
                menu.draw(res1366x768);
                menu.draw(res1366x768Text);
                menu.draw(res1024x576);
                menu.draw(res1024x576Text);
                menu.draw(res800x450);
                menu.draw(res800x450Text);

                menu.draw(musicVol0);
                menu.draw(musicVol0Text);
                menu.draw(musicVol25);
                menu.draw(musicVol25Text);
                menu.draw(musicVol50);
                menu.draw(musicVol50Text);
                menu.draw(musicVol100);
                menu.draw(musicVol100Text);

                menu.draw(sfxVol0);
                menu.draw(sfxVol0Text);
                menu.draw(sfxVol25);
                menu.draw(sfxVol25Text);
                menu.draw(sfxVol50);
                menu.draw(sfxVol50Text);
                menu.draw(sfxVol100);
                menu.draw(sfxVol100Text);

                menu.draw(delay01);
                menu.draw(delay01Text);
                menu.draw(delay05);
                menu.draw(delay05Text);
                menu.draw(delay10);
                menu.draw(delay10Text);
                menu.draw(delay20);
                menu.draw(delay20Text);

                menu.draw(txtDefault);
                menu.draw(txtDefaultText);
                menu.draw(txtDark);
                menu.draw(txtDarkText);
                menu.draw(txtClassic);
                menu.draw(txtClassicText);
                menu.draw(txtCustom);
                menu.draw(txtCustomText);

                menu.draw(optionsBackButton);
                menu.draw(optionsBackText);

                menu.draw(optionsApplyButton);
                menu.draw(optionsApplyText);
                break;

            case 3 :
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(playBackButton);
                menu.draw(playBackText);

                menu.draw(slot1);
                menu.draw(slot1Text);
                menu.draw(slot1DC);
                menu.draw(slot1DCText);

                menu.draw(slot2);
                menu.draw(slot2Text);
                menu.draw(slot2DC);
                menu.draw(slot2DCText);

                menu.draw(slot3);
                menu.draw(slot3Text);
                menu.draw(slot3DC);
                menu.draw(slot3DCText);

                menu.draw(slot4);
                menu.draw(slot4Text);
                menu.draw(slot4DC);
                menu.draw(slot4DCText);

                break;
            case 4:
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(loadingText);

                break;

            case 5:
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }

                menu.draw(deleteMsg);
                menu.draw(deleteMsgText);

                menu.draw(deleteMsgYes);
                menu.draw(deleteMsgYesText);

                menu.draw(deleteMsgNo);
                menu.draw(deleteMsgNoText);
                break;

            case 6:
                //Draw Background
                for(int x = 0; x < windowModeSizeX; x+=64)
                {
                    for(int y = 0; y < windowModeSizeY; y+=64)
                    {
                        bgSprite.setPosition(Vector2f(x,y));
                        menu.draw(bgSprite);
                    }
                }


                menu.draw(sizeMsg);
                menu.draw(sizeMsgText);

                menu.draw(size25);
                menu.draw(size25Text);
                menu.draw(size50);
                menu.draw(size50Text);
                menu.draw(size75);
                menu.draw(size75Text);
                menu.draw(size100);
                menu.draw(size100Text);
                break;


            }

            menu.display();
        }
    }
}

int loadCursor()
{
    cursorHitbox.setSize(Vector2f(64,64));
    cursorHitbox.setTexture(&emptyTexture);
    cursorHitbox.setOutlineColor(Color::Red);
    cursorHitbox.setOutlineThickness(4);
    cursorHitbox.setFillColor(Color(255, 255, 255, 100));

    cursorFont.loadFromFile("assets/" + texturePath + "/font.ttf");

    cursorText.setFont(cursorFont);
    cursorText.setCharacterSize(20);
    cursorText.setFillColor(Color::Black);
    cursorText.setString(cursorString);

    cursorComponent.setBlock("empty");
}

int updateCursor(Level &level)
{
    cursorX = (int)((mPos.x * cameraZoom + cameraCenterX )  / 64) * 64;
    cursorY = (int)((mPos.y * cameraZoom + cameraCenterY )  / 64) * 64;

    if(cursorX >= 64 && cursorX < (level.sizeX * 64) && cursorY >= 64 && cursorY < (level.sizeY * 64) && window.hasFocus())
    {
        cursorHitbox.setPosition(Vector2f(cursorX, cursorY));
        //Cursor Text:
        if(cursorComponent.type == "empty")
        {
            cursorString.clear();
            cursorText.setPosition(Vector2f(cursorX + 2, cursorY - 2));
            cursorString = std::to_string((int)cursorX/64) + "," + std::to_string((int)cursorY/64);
            cursorText.setString(cursorString);
        }

        drawCursor = true;
    }
    else
        drawCursor = false;

    bool ok=false;

    //place block:
    if(drawCursor && Mouse::isButtonPressed(Mouse::Left) && blockPlacementClock.getElapsedTime().asMilliseconds() >= 100 && window.hasFocus())
    {
        ok=false;
        if(cursorComponent.type != level.component[(int)cursorX/64][(int)cursorY/64].type || cursorComponent.rotation != level.component[(int)cursorX/64][(int)cursorY/64].rotation)
        {
            if(cursorComponent.type == "empty" && level.component[(int)cursorX/64][(int)cursorY/64].type != "wire" && level.component[(int)cursorX/64][(int)cursorY/64].type != "wireCorner" && level.component[(int)cursorX/64][(int)cursorY/64].type != "wire3way"  && level.component[(int)cursorX/64][(int)cursorY/64].type != "wire4way" && level.component[(int)cursorX/64][(int)cursorY/64].type != "wireJunction")
            {
                for(int i = 0; i < level.comp.size(); ++i)
                {
                    if(level.comp[i].first == (int)cursorX/64 && level.comp[i].second == (int)cursorY/64)
                    {
                        std::swap(level.comp[i],level.comp[level.comp.size()-1]);
                        ok=true;
                        break;
                    }
                }
                if(ok==true)
                {
                    level.comp.pop_back();
                }
            }
            else if(level.component[(int)cursorX/64][(int)cursorY/64].type != "empty")
            {
                ok=false;
                if(cursorComponent.type == "wire" || cursorComponent.type == "wireCorner" || cursorComponent.type == "wire3way" || cursorComponent.type == "wire4way"|| cursorComponent.type == "wireJunction")
                {
                    for(int i = 0; i < level.comp.size(); ++i)
                    {
                        if(level.comp[i].first == (int)cursorX/64 && level.comp[i].second == (int)cursorY/64)
                        {
                            std::swap(level.comp[i],level.comp[level.comp.size()-1]);
                            ok=true;
                            break;
                        }
                    }
                    if(ok==true)
                    {
                        level.comp.pop_back();
                    }

                }
                else if((level.component[(int)cursorX/64][(int)cursorY/64].type == "wire" || level.component[(int)cursorX/64][(int)cursorY/64].type == "wireCorner" || level.component[(int)cursorX/64][(int)cursorY/64].type == "wireJunction" || level.component[(int)cursorX/64][(int)cursorY/64].type == "wire3way" || level.component[(int)cursorX/64][(int)cursorY/64].type == "wire4way") && cursorComponent.type != "empty" )
                {
                    level.comp.push_back({(int)cursorX/64,(int)cursorY/64});
                }
            }
            else if(level.component[(int)cursorX/64][(int)cursorY/64].type == "empty" && cursorComponent.type != "wire" && cursorComponent.type != "wireCorner" && cursorComponent.type != "wire3way" && cursorComponent.type != "wireJunction" && cursorComponent.type != "wire4way")
            {
                level.comp.push_back({(int)cursorX/64,(int)cursorY/64});
            }
        }

        if(cursorComponent.type == "empty" && level.component[(int)cursorX/64][(int)cursorY/64].type != "empty")
            removeSound.play();
        if(cursorComponent.type != "empty" && level.component[(int)cursorX/64][(int)cursorY/64].type != cursorComponent.type)
            placeSound.play();


        level.component[(int)cursorX/64][(int)cursorY/64].setBlock(cursorComponent.type, cursorComponent.rotation);
        level.clearMap();
        level.update();
        blockPlacementClock.restart();
    }

    //rotate block:
    if(drawCursor && Keyboard::isKeyPressed(Keyboard::R) && blockPlacementClock.getElapsedTime().asMilliseconds() >= 150 && window.hasFocus() && cursorComponent.type != "empty")
    {
        cursorComponent.rotateBlock();
        blockPlacementClock.restart();
    }

    //rotate already existing block:
    if(drawCursor && window.hasFocus() && cursorComponent.type == "empty" && sf::Keyboard::isKeyPressed(Keyboard::R) &&  blockPlacementClock.getElapsedTime().asMilliseconds() >= 150 && level.component[(int)cursorX/64][(int)cursorY/64].type != "empty")
    {
        level.component[(int)cursorX/64][(int)cursorY/64].rotateBlock();
        //std::cout << "rotate" << std::endl;
        level.clearMap();
        level.update();
        blockPlacementClock.restart();
    }

    //Pick block:
    if(drawCursor && window.hasFocus() && blockPlacementClock.getElapsedTime().asMilliseconds() >= 150 && sf::Keyboard::isKeyPressed(Keyboard::Q))
    {
        cursorComponent.type = level.component[(int)cursorX/64][(int)cursorY/64].type;
        cursorComponent.rotation = level.component[(int)cursorX/64][(int)cursorY/64].rotation;
        cursorComponent.setBlock(cursorComponent.type, cursorComponent.rotation);
        blockPlacementClock.restart();
    }



    //Change switch:
    if(drawCursor && Mouse::isButtonPressed(Mouse::Right) && blockPlacementClock.getElapsedTime().asMilliseconds() >= 100 && window.hasFocus())
    {
        if(level.component[(int)cursorX/64][(int)cursorY/64].type == "switch")
        {
            level.component[(int)cursorX/64][(int)cursorY/64].isPowered = !level.component[(int)cursorX/64][(int)cursorY/64].isPowered;
            level.clearMap();
            level.update();
            blockPlacementClock.restart();
            switchSound.play();
        }
    }

}

//Pause Menu:
RenderWindow pauseWindow;
Texture bgTexture;
Sprite bgSprite;
RectangleShape resumeButton;
Text resumeText;
FloatRect resumeTextRect;
RectangleShape saveButton;
Text saveText;
FloatRect saveTextRect;

Vector2i mPosPause;


//Inventory:
Vector2i mPosInv;
RenderWindow inventoryWindow;
Texture inventoryTexture;
Sprite inventorySprite;
Component inventoryComponent[8][8];

RectangleShape inventoryCursor;

int loadInventory()
{
    inventoryTexture.loadFromFile("assets/" + texturePath + "/inventory.png");
    inventorySprite.setTexture(inventoryTexture);

    inventoryCursor.setSize(Vector2f(68, 68));
    inventoryCursor.setOutlineColor(Color::Blue);
    inventoryCursor.setTexture(&emptyTexture);
    inventoryCursor.setOutlineThickness(4);

    //Setup:
    for(int i = 0 ; i < 8; i++)
        for(int j =0 ; j < 8; j++)
            inventoryComponent[i][j].setBlock("empty");

    //First row:
    inventoryComponent[0][0].setBlock("empty");
    inventoryComponent[1][0].setBlock("switch");
    inventoryComponent[2][0].setBlock("screen");
    inventoryComponent[3][0].setBlock("wire", "right");
    inventoryComponent[4][0].setBlock("wireCorner", "right");
    inventoryComponent[5][0].setBlock("wire3way", "right");
    inventoryComponent[6][0].setBlock("wire4way");
    inventoryComponent[7][0].setBlock("wireJunction");

    //Second row:
    inventoryComponent[0][1].setBlock("NOTgate", "right");
    inventoryComponent[1][1].setBlock("ORgate", "right");
    inventoryComponent[2][1].setBlock("ANDgate", "right");
    inventoryComponent[3][1].setBlock("XORgate", "right");
    inventoryComponent[4][1].setBlock("XANDgate", "right");
    inventoryComponent[5][1].setBlock("NORgate", "right");
    inventoryComponent[6][1].setBlock("NANDgate", "right");

}

int updateInventory()
{
    if(Keyboard::isKeyPressed(Keyboard::I) && !inventoryWindow.isOpen())
    {

        if(Fullscreen == true)
        {
            inventoryWindow.create(VideoMode(548, 548), "Inventory", Style::None);
            inventoryWindow.setPosition(Vector2i(window.getPosition().x + window.getSize().x/2 - inventoryWindow.getSize().x/2, window.getPosition().y + window.getSize().y/2 - inventoryWindow.getSize().y/2));
            HWND inventoryHandle = inventoryWindow.getSystemHandle();
            HWND windowHandle = window.getSystemHandle();
            SetParent(inventoryHandle,windowHandle);

            SetWindowLongPtr(inventoryWindow.getSystemHandle(), GWL_EXSTYLE,
                             GetWindowLongPtr(inventoryWindow.getSystemHandle(), GWL_EXSTYLE)
                             | WS_EX_TOOLWINDOW);
        }
        else if(Fullscreen == false)
        {
            inventoryWindow.create(VideoMode(548, 548), "Inventory", Style::None);
            inventoryWindow.setPosition(Vector2i(window.getPosition().x + window.getSize().x/2 - inventoryWindow.getSize().x/2, window.getPosition().y + window.getSize().y/2 - inventoryWindow.getSize().y/2));

            SetWindowLongPtr(inventoryWindow.getSystemHandle(), GWL_EXSTYLE,
                             GetWindowLongPtr(inventoryWindow.getSystemHandle(), GWL_EXSTYLE)
                             | WS_EX_TOOLWINDOW);
        }
    }


    if(inventoryWindow.isOpen() && inventoryWindow.hasFocus())
    {
        mPosInv = Mouse::getPosition(inventoryWindow);
        inventoryCursor.setPosition(Vector2f((int)(mPosInv.x / 68) * 68, (int)(mPosInv.y / 68) * 68));

        inventoryWindow.clear(sf::Color::Black);
        inventoryWindow.draw(inventorySprite);
        if((int)(mPosInv.y) > 0 && (int)(mPosInv.x) < 68*8 && (int)(mPosInv.y) < 68*8)
            inventoryWindow.draw(inventoryCursor);
        inventoryWindow.display();


        if(Mouse::isButtonPressed(Mouse::Left) && (int)(mPosInv.x) > 0 && (int)(mPosInv.y) > 0 && (int)(mPosInv.x) < 68*8 && (int)(mPosInv.y) < 68*8)
        {
            cursorComponent.setBlock(inventoryComponent[(int)(mPosInv.x / 68)][(int)(mPosInv.y / 68)].type, inventoryComponent[(int)(mPosInv.x / 68)][(int)(mPosInv.y / 68)].rotation);
            blockPlacementClock.restart();
            window.requestFocus();
            inventoryWindow.close();
        }
    }

    if(inventoryWindow.isOpen() && !inventoryWindow.hasFocus())
    {
        inventoryWindow.close();
    }
    if(inventoryWindow.isOpen() && Keyboard::isKeyPressed(Keyboard::Escape))
    {
        inventoryWindow.close();
        Sleep(500);
    }
}

int loadPauseMenu()
{
    bgTexture.loadFromFile("assets/" + texturePath + "/Menu/floor.png");
    bgSprite.setTexture(bgTexture);

    //resumeButton:
    resumeButton.setSize(Vector2f(200, 80));
    resumeButton.setOrigin(Vector2f(resumeButton.getLocalBounds().width/2, resumeButton.getGlobalBounds().height/2));
    resumeButton.setPosition(Vector2f(256/2, 256/4 + 4));
    resumeButton.setFillColor(Color(96, 184, 84));
    resumeButton.setOutlineColor(Color(255,255,255));
    resumeButton.setOutlineThickness(4);
    resumeText.setFont(font);
    resumeText.setString("Resume");
    resumeText.setCharacterSize(40);
    resumeText.setFillColor(Color(255,255,255));
    resumeTextRect = resumeText.getLocalBounds();
    resumeText.setOrigin(resumeTextRect.left+resumeTextRect.width/2.0f,resumeTextRect.top+resumeTextRect.height/2.0f);
    resumeText.setPosition(Vector2f(256/2, 256/4 + 4));

    //saveButton:
    saveButton.setSize(Vector2f(200, 80));
    saveButton.setOrigin(Vector2f(saveButton.getLocalBounds().width/2, saveButton.getGlobalBounds().height/2));
    saveButton.setPosition(Vector2f(256/2, 256/1.4 + 6));
    saveButton.setFillColor(Color(222, 55, 55));
    saveButton.setOutlineColor(Color(255,255,255));
    saveButton.setOutlineThickness(4);
    saveText.setFont(font);
    saveText.setString("Save&Exit");
    saveText.setCharacterSize(40);
    saveText.setFillColor(Color(255,255,255));
    saveTextRect = saveText.getLocalBounds();
    saveText.setOrigin(saveTextRect.left+saveTextRect.width/2.0f,saveTextRect.top+saveTextRect.height/2.0f);
    saveText.setPosition(Vector2f(256/2, 256/1.4 + 6));

}

int updatePauseMenu(Level &level)
{
    mPosPause = Mouse::getPosition(pauseWindow);

    if(pauseWindow.isOpen())
    {
        pauseWindow.clear(Color::Black);

        //Draw Background
        for(int x = 0; x < 256; x+=64)
        {
            for(int y = 0; y < 256; y+=64)
            {
                bgSprite.setPosition(Vector2f(x,y));
                pauseWindow.draw(bgSprite);
            }
        }

        pauseWindow.draw(resumeButton);
        pauseWindow.draw(resumeText);

        pauseWindow.draw(saveButton);
        pauseWindow.draw(saveText);
        pauseWindow.display();

    }

    //Controls:
    if(Keyboard::isKeyPressed(Keyboard::Escape) && !inventoryWindow.isOpen() && !pauseWindow.isOpen())
    {
        if(Fullscreen == true)
        {
            pauseWindow.create(VideoMode(256,256), "Pause Menu", Style::None);
            pauseWindow.setPosition(Vector2i(window.getPosition().x + window.getSize().x/2 - pauseWindow.getSize().x/2, window.getPosition().y + window.getSize().y/2 - pauseWindow.getSize().y/2));

            HWND pauseHandle = pauseWindow.getSystemHandle();
            HWND windowHandle = window.getSystemHandle();
            SetParent(pauseHandle,windowHandle);

            SetWindowLongPtr(pauseWindow.getSystemHandle(), GWL_EXSTYLE,
                             GetWindowLongPtr(pauseWindow.getSystemHandle(), GWL_EXSTYLE)
                             | WS_EX_TOOLWINDOW);
        }
        else if(Fullscreen == false)
        {
            pauseWindow.create(VideoMode(256,256), "Pause Menu", Style::None);
            pauseWindow.setPosition(Vector2i(window.getPosition().x + window.getSize().x/2 - pauseWindow.getSize().x/2, window.getPosition().y + window.getSize().y/2 - pauseWindow.getSize().y/2));

            SetWindowLongPtr(pauseWindow.getSystemHandle(), GWL_EXSTYLE,
                             GetWindowLongPtr(pauseWindow.getSystemHandle(), GWL_EXSTYLE)
                             | WS_EX_TOOLWINDOW);

        }
    }
    if(Mouse::isButtonPressed(Mouse::Left) && resumeButton.getGlobalBounds().contains(mPosPause.x, mPosPause.y))
    {
        pauseWindow.close();
        Sleep(500);
    }
    if(Mouse::isButtonPressed(Mouse::Left) && saveButton.getGlobalBounds().contains(mPosPause.x, mPosPause.y))
    {

        pauseWindow.close();
        window.close();
        if(errorCode == 1)
        {
            saveMap("slot1.map",level);
        }
        if(errorCode == 2)
        {
            saveMap("slot2.map",level);
        }
        if(errorCode == 3)
        {
            saveMap("slot3.map",level);
        }
        if(errorCode == 4)
        {
            saveMap("slot4.map",level);
        }

        return -2;
    }


    if(!pauseWindow.hasFocus())
    {
        pauseWindow.close();
    }
}




int loadTextures()
{
    //General Textures:
    floorTexture.loadFromFile("assets/" + texturePath + "/floor.png");
    switchOnTexture.loadFromFile("assets/" + texturePath + "/components/switchOn.png");
    switchOffTexture.loadFromFile("assets/" + texturePath + "/components/switchOff.png");
    emptyTexture.loadFromFile("assets/" + texturePath + "/components/empty.png");
    screenOnTexture.loadFromFile("assets/" + texturePath + "/components/screenOn.png");
    screenOffTexture.loadFromFile("assets/" + texturePath + "/components/screenOff.png");

    //Wire Textures:
    wireOnrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/wireOnright.png");
    wireOndownTexture.loadFromFile("assets/" + texturePath + "/components/wires/wireOndown.png");
    wireOffrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/wireOffright.png");
    wireOffdownTexture.loadFromFile("assets/" + texturePath + "/components/wires/wireOffdown.png");

    //WireCorner Textures:
    wireCornerOffrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOffright.png");
    wireCornerOffdownTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOffdown.png");
    wireCornerOffleftTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOffleft.png");
    wireCornerOffupTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOffup.png");
    wireCornerOnrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOnright.png");
    wireCornerOndownTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOndown.png");
    wireCornerOnleftTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOnleft.png");
    wireCornerOnupTexture.loadFromFile("assets/" + texturePath + "/components/wires/cornerOnup.png");

    //Wire 3-way Texture:
    wire3wayOffrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOffright.png");
    wire3wayOffdownTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOffdown.png");
    wire3wayOffleftTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOffleft.png");
    wire3wayOffupTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOffup.png");
    wire3wayOnrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOnright.png");
    wire3wayOndownTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOndown.png");
    wire3wayOnleftTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOnleft.png");
    wire3wayOnupTexture.loadFromFile("assets/" + texturePath + "/components/wires/3wayOnup.png");

    //Wire 4-way Texture:
    wire4wayOffrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/4wayOff.png");
    wire4wayOnrightTexture.loadFromFile("assets/" + texturePath + "/components/wires/4wayOn.png");

    //Wire junction:
    wireJunctionAlloff.loadFromFile("assets/" + texturePath + "/components/wires/junctionAlloff.png");
    wireJunctionAllpowered.loadFromFile("assets/" + texturePath + "/components/wires/junctionAllpowered.png");
    wireJunctionVerticalpowered.loadFromFile("assets/" + texturePath + "/components/wires/junctionVerticalpowered.png");
    wireJunctionHorizontalpowered.loadFromFile("assets/" + texturePath + "/components/wires/junctionHorizontalpowered.png");


    //NOTGate Textures:
    NOTrightTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NOTright.png");
    NOTdownTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NOTdown.png");
    NOTleftTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NOTleft.png");
    NOTupTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NOTup.png");

    //ORgate Textures:
    ORupTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ORup.png");
    ORdownTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ORdown.png");
    ORrightTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ORright.png");
    ORleftTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ORleft.png");

    //ANDgate Textures:
    ANDupTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ANDup.png");
    ANDdownTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ANDdown.png");
    ANDrightTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ANDright.png");
    ANDleftTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/ANDleft.png");

    //XORgate Textures:
    XORupTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XORup.png");
    XORdownTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XORdown.png");
    XORrightTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XORright.png");
    XORleftTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XORleft.png");

    //XANDgate Textures:
    XANDupTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XANDup.png");
    XANDdownTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XANDdown.png");
    XANDrightTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XANDright.png");
    XANDleftTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/XANDleft.png");

    //NORgate Textures:
    NORupTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NORup.png");
    NORdownTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NORdown.png");
    NORrightTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NORright.png");
    NORleftTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NORleft.png");

    //NANDgate Textures:
    NANDupTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NANDup.png");
    NANDdownTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NANDdown.png");
    NANDrightTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NANDright.png");
    NANDleftTexture.loadFromFile("assets/" + texturePath + "/components/logicGates/NANDleft.png");
}

int render(Level &level)
{
    window.clear(Color::Black);

    //Map drawing:
    for(int x = 1; x < level.sizeX; x++)
    {
        for(int y = 1; y < level.sizeY; y++)
        {
            if((x * 64) >= (cameraCenterX - 64) && (x * 64) <= cameraCornerX && ((y * 64) >= (cameraCenterY - 64) && (y * 64) <= cameraCornerY))
            {
                level.floor.setPosition(Vector2f(x*64, y*64));
                window.draw(level.floor);


                level.hitbox.setPosition(Vector2f(x*64, y*64));
                level.setTexture(&level.hitbox, level.component[x][y].type, level.component[x][y].rotation, level.component[x][y].isPowered, level.component[x][y].isPowered2);
                window.draw(level.hitbox);
            }
        }
    }

    //Cursor:
    if(drawCursor)
    {
        level.setTexture(&cursorHitbox, cursorComponent.type, cursorComponent.rotation, cursorComponent.isPowered, cursorComponent.isPowered2);
        window.draw(cursorHitbox);
        if(cursorComponent.type == "empty")
        {
            window.draw(cursorText);
        }
    }



    //Render:
    window.display();
}

int updateCamera(Level &level)
{
    cameraSizeX = (int)(windowSizeX * cameraZoom);
    cameraSizeY = (int)(windowSizeY * cameraZoom);

    cameraCornerX = cameraCenterX + cameraSizeX;
    cameraCornerY = cameraCenterY + cameraSizeY;

    //Movement:
    if(Keyboard::isKeyPressed(Keyboard::D) && window.hasFocus())
    {
        cameraCenterX += cameraSpeed * deltaTime;
    }
    if(Keyboard::isKeyPressed(Keyboard::S) && window.hasFocus())
    {
        cameraCenterY += cameraSpeed * deltaTime;
    }
    if(Keyboard::isKeyPressed(Keyboard::A) && window.hasFocus())
    {
        cameraCenterX -= cameraSpeed * deltaTime;
    }
    if(Keyboard::isKeyPressed(Keyboard::W) && window.hasFocus())
    {
        cameraCenterY -= cameraSpeed * deltaTime;
    }
    if(Keyboard::isKeyPressed(Keyboard::F11))
    {
        window.close();
        if(Fullscreen == false)
        {
            windowSizeX = VideoMode::getDesktopMode().width;
            windowSizeY = VideoMode::getDesktopMode().height;
            window.create(VideoMode(windowSizeX, windowSizeY), "Computation", sf::Style::Fullscreen), Fullscreen = true;
            window.setFramerateLimit(100);
            saveConfig();
        }
        else if(Fullscreen == true)
        {
            windowSizeX = windowModeSizeX;
            windowSizeY = windowModeSizeY;
            window.create(VideoMode(windowSizeX, windowSizeY), "Computation", sf::Style::Close), Fullscreen = false;
            window.setFramerateLimit(100);
            saveConfig();
        }
        Sleep(100);
    }

    //Zoom:
    if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::Equal) && cameraSizeX > cameraSizeMinX && cameraSizeY > cameraSizeMinY)
    {
        cameraZoom -= cameraZoomSpeed * deltaTime;
    }
    if(Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::Hyphen) && cameraSizeX < cameraSizeMaxX && cameraSizeY < cameraSizeMaxY)
    {
        cameraZoom += cameraZoomSpeed * deltaTime;
    }


    camera.reset(FloatRect(Vector2f(cameraCenterX, cameraCenterY), Vector2f(cameraSizeX, cameraSizeY)));
    window.setView(camera);
}

int update(Level &level)
{
    //General updates:
    deltaTime = frameClock.restart().asMilliseconds();
    mPos = Mouse::getPosition(window);

    //Updates:
    updateCursor(level);
    updateCamera(level);
    updateInventory();
    int ok_2 = updatePauseMenu(level);

    if(ok_2 == -2)
        return -2;



    if(levelUpdateClock.getElapsedTime().asSeconds() >= levelUpdateDelay)
    {
        levelUpdateClock.restart();
        level.update();
    }


}

int main()
{

    loadConfig();

    bgMusic.openFromFile(("assets/" + texturePath + "/sound/music.wav"));
    bgMusic.setLoop(true);
    bgMusic.play();
    bgMusic.setVolume(musicVolume*100);

    placeSoundBuf.loadFromFile("assets/" + texturePath + "/sound/place.wav");
    removeSoundBuf.loadFromFile("assets/" + texturePath + "/sound/remove.wav");
    switchSoundBuf.loadFromFile("assets/" + texturePath + "/sound/switch.wav");

    placeSound.setBuffer(placeSoundBuf);
    removeSound.setBuffer(removeSoundBuf);
    switchSound.setBuffer(switchSoundBuf);

    placeSound.setVolume(sfxVolume*100);
    removeSound.setVolume(sfxVolume*100);
    switchSound.setVolume(sfxVolume*100);


Program:


    //Setup:
    errorCode = mainMenu();

    placeSound.setVolume(sfxVolume*100);
    removeSound.setVolume(sfxVolume*100);
    switchSound.setVolume(sfxVolume*100);

    //std::cout << errorCode << " " << saveSlot;

    if(errorCode == -1)
    {
        return -1;
    }

    if(Fullscreen == true)
    {
        windowSizeX = VideoMode::getDesktopMode().width;
        windowSizeY = VideoMode::getDesktopMode().height;
        window.create(VideoMode(windowSizeX, windowSizeY), "Computation", sf::Style::Fullscreen);
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        window.setFramerateLimit(100);
    }
    else if(Fullscreen == false)
    {
        windowSizeX = windowModeSizeX;
        windowSizeY = windowModeSizeY;
        window.create(VideoMode(windowSizeX, windowSizeY), "Computation", sf::Style::Close);
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        window.setFramerateLimit(100);
    }
    Sleep(100);
    window.setFramerateLimit(100);




    loadTextures();
    loadCursor();
    loadInventory();
    loadPauseMenu();

    cameraCenterX = 0.0f;
    cameraCenterY = 0.0f;
    cameraCornerX = 0.0f;
    cameraCornerY = 0.0f;


    if(errorCode == 1)
    {
        loadMapSize("slot1.map");
    }
    if(errorCode == 2)
    {
        loadMapSize("slot2.map");
    }
    if(errorCode == 3)
    {
        loadMapSize("slot3.map");
    }
    if(errorCode == 4)
    {
        loadMapSize("slot4.map");
    }



    Level level(mapSize+1, mapSize+1);




    if(errorCode == 1)
    {
        loadMap("slot1.map",level);
    }
    if(errorCode == 2)
    {
        loadMap("slot2.map",level);
    }
    if(errorCode == 3)
    {
        loadMap("slot3.map",level);
    }
    if(errorCode == 4)
    {
        loadMap("slot4.map",level);
    }


    /*for(int x = 0; x < 50; x++){
        for(int y= 0; y < 50; y++)
        {
            level.component[x][y].setBlock("NOTgate", "down");
            level.comp.push_back({x, y});
        }
    }*/

    //main loop:
    while(window.isOpen())
    {
        while(window.pollEvent(windowEvent))
        {
            if(windowEvent.type == Event::Closed)
            {
                if(errorCode == 1)
                {
                    saveMap("slot1.map",level);
                }
                if(errorCode == 2)
                {
                    saveMap("slot2.map",level);
                }
                if(errorCode == 3)
                {
                    saveMap("slot3.map",level);
                }
                if(errorCode == 4)
                {
                    saveMap("slot4.map",level);
                }
                window.close();
            }
        }

        //Update:
        ok_3 = update(level);
        render(level);
        if(ok_3 == -2)
        {
            window.close();
            goto Program;
        }

    }

}
