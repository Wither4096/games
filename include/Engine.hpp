#pragma once

//just include everything why the fuck not
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<string>
#include<vector>
#include<unistd.h>

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>

class Engine{
    private:
    //Variables
    sf::RenderWindow* window;

    sf::Font textFont;

    //framerate agnostic consistent timing 
    sf::Clock clock;
    sf::Time deltaTime;
    float dtSeconds;

    //Define the resoultion of the window by taking the desktop res. and halving
    sf::VideoMode desktopVMode = sf::VideoMode::getDesktopMode();
    unsigned int desktopXSize=desktopVMode.size.x;
    unsigned int desktopYSize=desktopVMode.size.y;
    sf::VideoMode windowVMode = sf::VideoMode({desktopXSize/2,desktopYSize/2});
    //get the individual size of components of window
    unsigned int windowXSize=windowVMode.size.x;
    unsigned int windowYSize=windowVMode.size.y;
    //define play area as 80% of window
    int playAreaBoundMaxX=windowXSize*0.80;
    int playAreaBoundMaxY=windowYSize*0.80;

    //Mouse Position Relative to window
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //TODO SPLIT
    //entity related
    sf::RectangleShape entity;
    std::vector<sf::RectangleShape> entityVect;
    float entitySpawnTimer;
    float entitySpawnTimerMax;
    float grav;
    int maxEntities;
    const int healthMax=10;
    int health;
    unsigned int points;
    bool gameEnd;

    //inti Funtions
    void initVariables();
    void initWindow();

public:
    //Constructor / Destructor
    Engine();
    ~Engine();

    //Getters
    const bool getIsRunning()const;
    float getDeltaTime();
    const bool getGameStatusEnd()const;

    //Setters
    void setMousePosWindow();
    void setDeltaTimeSeconds();

    //Functions
    void pollEvents();

    void centerWindow(); //pretty useless actually

    //TODO SPLIT
    //entity related
    void spawnEntity();
    void updateEntity();
    void renderEntity();
    void applyEntityGravity(float grav,float dtSeconds);
    void ifMouseClicked();
    void deleteOOB();
    void updateHealth(bool skillChecked=false);
    void updatePoints();

    void displayScore();
    void displayFramerate();
    void displayHealth();
    void displayGameOver();

    void update();
    void render();
};