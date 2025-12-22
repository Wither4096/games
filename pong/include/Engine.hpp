#pragma once
#include<iostream>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
class Engine{
    private:
    sf::RenderWindow* window;
    sf::Font textFont;
    sf::Clock clock;
    sf::Time deltaTime;
    float deltaSeconds;
    sf::VideoMode windowMode;
    void initVariables();
    void initWindow();
    public:
    Engine();
    ~Engine();
    void setDeltaTime();
    float getDeltaTime();
    bool getEngineRunning();
    void engineUpdate();
    void engineRender();
};