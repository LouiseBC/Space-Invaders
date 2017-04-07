#ifndef GameState_hpp
#define GameState_hpp
#include "Game.hpp"

class GameState {
public:
    virtual bool init(Graphics* graphics, Game* game)  = 0;
    virtual ~GameState() {};
    
    virtual void handleEvents(SDL_Event& event) = 0;
    virtual void update(const float& deltatime) = 0;
    virtual void render() = 0;
};

#endif