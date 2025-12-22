#include"../include/Engine.hpp"
Engine::Engine(){
    initVariables();
    initWindow();
}

Engine::~Engine(){
    delete window;
}

void Engine::initVariables(){
    textFont=sf::Font("data/fonts/JetBrainsMono-Regular.ttf");
}

void Engine::initWindow(){
    window=new sf::RenderWindow(sf::VideoMode::getDesktopMode(),"Pong",sf::Style::Titlebar|sf::Style::Close,sf::State::Fullscreen);
    //window->setPosition({0,0});
}

void Engine::setDeltaTime(){
    deltaTime=clock.reset();
    deltaSeconds=deltaTime.asSeconds();
}

float Engine::getDeltaTime(){
    return deltaSeconds;
}

bool Engine::getEngineRunning(){
    return window->isOpen();
}

void Engine::engineUpdate(){
    while(const std::optional event=window->pollEvent()){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        window->close();
    }
    engineRender();
}

void Engine::engineRender(){
    window->clear();
    window->display();
}