#ifndef MenuState_hpp
#define MenuState_hpp
#include "GameState.hpp"

class MenuState : public GameState {
public:
    bool init(Graphics* graphics, Game* game);
    ~MenuState();
    
    void handleEvents(SDL_Event& event);
    void update(const float& deltatime);
    void render();
    void renderTitle();
    void renderControls();
private:
    Graphics* graphics { nullptr };
    Game* game         { nullptr };
    
    SDL_Color greenColour { 41, 250, 46, 1 };
    SDL_Color whiteColour { 255, 255, 255, 1 };
    
    SDL_Texture* title       { nullptr };
    SDL_Rect     titleRect;
    SDL_Texture* title2      { nullptr };
    SDL_Rect     titleRect2;
    SDL_Rect     titleRect3;
    SDL_Texture* startInstr  { nullptr };
    SDL_Rect     startInstrRect;
    SDL_Texture* controls    { nullptr };
    SDL_Rect     controlsRect;
    SDL_Texture* controls2    { nullptr };
    SDL_Rect     controls2Rect;
};

#endif