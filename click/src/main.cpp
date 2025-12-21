#include"../include/Engine.hpp"

int main(){
    srand(time(NULL));
    Engine game;
    //Game Loop
    game.centerWindow();
    while(game.getIsRunning()){
        //Update states
        game.update();
        //Render updated states
        game.render();
    }
    return 0;
}