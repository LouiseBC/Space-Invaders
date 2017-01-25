#include "Game.hpp"
#include "MainGameState.hpp"
#include "MenuState.hpp"

int main() {
    Game game;
    if (game.init(new MenuState)) // Will be changed to MenuState when completed
        game.loop();
}
