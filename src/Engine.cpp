#include"../include/Engine.hpp"

void Engine::initVariables(){
    window=nullptr;
    points=0;
    entitySpawnTimer=0.f;
    entitySpawnTimerMax=30.f;
    grav=9.81;
    maxEntities=10;
    health=10;
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

float Engine::getDeltaTime(){
    return dtSeconds;
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

    float xSize = (rand()%75+1)+25;
    float ySize = (rand()%75+1)+25;
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
            window->close();
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
            entitySpawnTimer+=(60*getDeltaTime());
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
    points+=1;
    if(points%50==0){
        grav*=1.5;
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
    switch(health){
        case 10: case 9: case 8:
        healthDisplayText.setFillColor(sf::Color::Green);break;
        case 7: case 6: case 5: case 4:
        healthDisplayText.setFillColor(sf::Color::Yellow);break;
        case 3: case 2: case 1:
        healthDisplayText.setFillColor(sf::Color::Red);break;
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
    pollEvents();
    setDeltaTimeSeconds();
    setMousePosWindow();
    applyEntityGravity(((grav/4.0)*60.0),getDeltaTime());
    ifMouseClicked();
    deleteOOB();
    updateEntity();
}

void Engine::renderEntity(){
    for(auto e:entityVect){
        window->draw(e);
    }
}

void Engine::render(){
    window->clear();
    renderEntity();
    displayScore();
    displayHealth();
    displayFramerate();
    window->display();
}