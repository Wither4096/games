#include"../include/Engine.hpp"

void Engine::initVariables(){
    window=nullptr;
    points=0;
    entitySpawnTimer=0.f;
    entitySpawnTimerMax=30.f;
    grav=9.81;
    maxEntities=10;
    health=10;
    gameEnd=false;
    textFont=sf::Font("data/fonts/JetBrainsMono-Regular.ttf");
}

void Engine::initWindow(){
    window = new sf::RenderWindow(windowVMode,"Click!",sf::Style::Titlebar|sf::Style::Close,sf::State::Windowed);
    //window->setFramerateLimit(60); //useless after implementing deltaTime
}

Engine::Engine(){
    initVariables();
    initWindow();
}
Engine::~Engine(){
    delete window;
}
//Setters
void Engine::setDeltaTimeSeconds(){
    deltaTime=clock.restart();
    dtSeconds=deltaTime.asSeconds();
}

void Engine::setMousePosWindow(){
    //update mouse position relative to window
    mousePosWindow=sf::Mouse::getPosition(*window);
    mousePosView=window->mapPixelToCoords(mousePosWindow);
}

//Getters
const bool Engine::getIsRunning()const{
    return window->isOpen();
}

float Engine::getDeltaTime(){//Note: this gives the time since last frame was rendered, to convert from units per frame to units per second, multiply by 60
    return dtSeconds;
}

const bool Engine::getGameStatusEnd()const{
    return gameEnd;
}

void Engine::pollEvents(){ //poll Loop
    while(const std::optional event = window->pollEvent()){
        if(event->is<sf::Event::Closed>()){
            window->close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
            window->close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)){
            centerWindow();
        }
    }
}

void Engine::centerWindow(){
    //center the main window relative to display resolution
    int xDiff = desktopVMode.size.x - windowVMode.size.x;
    int yDiff = desktopVMode.size.y - windowVMode.size.y;
    window->setPosition({xDiff/2,yDiff/2});
}

void Engine::spawnEntity(){
    //set random colours for square fill, complenetary colours for fill
    int red=rand()%256;
    int green=rand()%256;
    int blue=rand()%256;
    entity.setFillColor(sf::Color(red,green,blue));
    entity.setOutlineColor(sf::Color(255-red,255-green,255-blue));
    entity.setOutlineThickness(5.f);

    const int maxSize=100;
    const int minSize=25;
    float xSize = (rand()%(maxSize-minSize)+1)+minSize;
    float ySize = (rand()%(maxSize-minSize)+1)+minSize;
    entity.setSize({xSize,ySize});

    //ensure random position is within bounds, 80% of the entire window
    entity.setPosition({
        //this code fucking sucks actually
        rand()%(playAreaBoundMaxX-(windowXSize-playAreaBoundMaxX)+1)+(windowXSize-playAreaBoundMaxX), //ignore warnings just trust me vro 💜
        0
        //rand()%(playAreaBoundMaxY-(windowYSize-playAreaBoundMaxY)+1)+(windowYSize-playAreaBoundMaxY)
    });
    //add to the vector
    entityVect.push_back(entity);
}

void Engine::updateHealth(bool skillChecked){
    if(skillChecked){
        if(health<healthMax){
            health+=1;
        }
    }
    else{
        health-=1;
        if(health==0){
            gameEnd=true;
        }
    }
}

void Engine::updateEntity(){
    //spawn an entity every second using deltaTime
    if(maxEntities>entityVect.size()){
        if(entitySpawnTimer>=entitySpawnTimerMax){
            spawnEntity();
            entitySpawnTimer=0.f;
        }
        else{
            entitySpawnTimer+=(60.0*getDeltaTime());
        }
    }
}

void Engine::applyEntityGravity(float grav,float dtSeconds){
    for(auto &e:entityVect){
        e.move({0,grav*dtSeconds});//gravity constant * time
    }
}

void Engine::ifMouseClicked(){
    for(int i=0;i<entityVect.size();i++){
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            if(entityVect[i].getGlobalBounds().contains(mousePosView)){
                entityVect.erase(entityVect.begin()+i);
                updatePoints();
                updateHealth(true);
            }
        }
    }
}

void Engine::updatePoints(){
    const float speedIncreaseFactor=1.5;
    const int difficultyIncreaseThreshold=50;
    points+=1;
    if(points%difficultyIncreaseThreshold==0){
        grav*=speedIncreaseFactor;
    }
}

void Engine::displayScore(){
    std::string gameScore=std::to_string(points);
    sf::Text scoreText(textFont,"Score: "+gameScore,32);

    scoreText.setPosition({windowXSize/2.0-130,0}); //also ignore this
    window->draw(scoreText);
}

void Engine::displayFramerate(){
    int frames=1.0f/getDeltaTime();
    std::string frameRate=std::to_string(frames);
    sf::Text frameRateText(textFont,"FPS: "+frameRate,32);

    frameRateText.setPosition({windowXSize-224,0}); //also ALSO ignore this
    window->draw(frameRateText);
}

void Engine::displayHealth(){
    std::string healthText=std::to_string(health);
    sf::Text healthDisplayText(textFont,"Health: "+healthText,32);
    if(health>=8){
        healthDisplayText.setFillColor(sf::Color::Green);
    }else if(health<=7&&health>=4){
        healthDisplayText.setFillColor(sf::Color::Yellow);
    }else{
        healthDisplayText.setFillColor(sf::Color::Red);
    }
    window->draw(healthDisplayText);
}

void Engine::deleteOOB(){
    for(int i=0;i<entityVect.size();i++){
        if(entityVect[i].getGlobalBounds().position.y>windowYSize){
            entityVect.erase(entityVect.begin()+i);
            updateHealth();
        }
    }
}

void Engine::update(){
    if(gameEnd){
        return;
    }else{
        pollEvents();
        setDeltaTimeSeconds();
        setMousePosWindow();
        applyEntityGravity((60.0*(grav/4.0)),getDeltaTime());
        ifMouseClicked();
        deleteOOB();
        updateEntity();
    }
}

void Engine::renderEntity(){
    for(auto e:entityVect){
        window->draw(e);
    }
}

void Engine::displayGameOver(){
    sf::Text gameOver(textFont,"GAME OVER",64);
    sf::Text playAgain(textFont,"Press R to play again",32);
    sf::Text quitGame(textFont,"or Esc to quit",24);
    gameOver.setPosition({windowXSize/2-(gameOver.getGlobalBounds().getCenter().x),windowYSize/2-(gameOver.getGlobalBounds().getCenter().y)});
    playAgain.setPosition({windowXSize/2-(playAgain.getGlobalBounds().getCenter().x),windowYSize/2-(playAgain.getGlobalBounds().getCenter().y)+(gameOver.getCharacterSize())});
    quitGame.setPosition({windowXSize/2-(quitGame.getGlobalBounds().getCenter().x),windowYSize/2-(quitGame.getGlobalBounds().getCenter().y)+(playAgain.getCharacterSize()+gameOver.getCharacterSize())});
    //^ FUCK THIS POSITONING 
    //^^FIX IT LATER
    window->clear();
    window->draw(gameOver);
    window->draw(playAgain);
    window->draw(quitGame);
    window->display();
    if(health<=0){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)){
        gameEnd=false;
        health=healthMax;
        points=0;
        window->clear();
        entityVect.clear();
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
        window->close();
    }
}

void Engine::render(){
    if(gameEnd){
        displayGameOver();
    }
    else{
        window->clear();
        renderEntity();
        displayScore();
        displayHealth();
        displayFramerate();
        window->display();
    }
}