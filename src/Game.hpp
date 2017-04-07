#ifndef Game_hpp
#define Game_hpp
#include "Graphics.hpp"

class GameState;

class Game {
public:
    bool init(GameState *state);
    void loop();
    
    void changeState(GameState *state);
    void setQuit();
private:
    void update();
    void render();
    void quitGame();
    
    bool quit    { false };
    GameState* currentState  { nullptr };
    
    Graphics graphics;
    SDL_Event event;
    
    Uint32 oldTime  { 0 };
    Uint32 currentTime { 0 };
};

#endif
