#include"../include/Engine.hpp"
int main(){
    Engine game;
    while(game.getEngineRunning()){
        game.engineUpdate();
        game.engineRender();
    }
    return 0;
}