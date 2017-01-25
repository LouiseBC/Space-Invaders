#ifndef HUD_hpp
#define HUD_hpp
#include <SDL2/SDL.h>

class Graphics;

class HUD {
public:
    void init(Graphics* graphics);
    void render();
    void renderScore(const int& playerscore);
    void renderPause();
    void renderGameOver();
    void renderLives(const int& lifecount);
private:
    Graphics* graphics;
    
    SDL_Color greenColour { 41, 250, 46, 1 };
    SDL_Color fontColour  { 225, 225, 225, 1 };
    
    SDL_Texture* score;
    SDL_Texture* currentScore;
    SDL_Rect scoreRect;
    SDL_Rect currentScoreRect;
    
    SDL_Texture* lives;
    SDL_Rect livesRect;
    
    SDL_Texture* gameOver;
    SDL_Texture* restartInstr;
    SDL_Texture* pause;
    SDL_Rect gameOverRect;
    SDL_Rect restartInstrRect;
};

#endif
