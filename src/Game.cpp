#include "Game.hpp"
#include "Gamestate.hpp"
#include <iostream>

bool Game::init(GameState *state) {
    if (graphics.setup())
        changeState(state);
    return true;
}

void Game::changeState(GameState *state) {
    if (currentState != nullptr)
        delete currentState;
    
    currentState = state;
    if (currentState->init(&graphics, this) == false) {
        std::cerr << "State init error" << std::endl;
        quit = true;
    }
}

void Game::loop()
{
    while (quit == false)
    {
        update();
        render();
    }
    quitGame();
}

void Game::update() {
    if(currentState != nullptr){
        while(SDL_PollEvent(&event))
            currentState->handleEvents(event);
    }
    
    if (quit == false){
        oldTime = currentTime;
        currentTime = SDL_GetTicks();
        float deltaTime = (float)(currentTime - oldTime) / 1000.0f;
        currentState->update(deltaTime);
    }
}

void Game::render() {
    currentState->render();
}

void Game::quitGame() {
    delete currentState;
    graphics.destroy();
    SDL_Quit();
}

void Game::setQuit() {
    quit = true;
}