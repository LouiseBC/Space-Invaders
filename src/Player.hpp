#ifndef Player_hpp
#define Player_hpp
#include "Graphics.hpp"
#include "Bullet.hpp"
#include <vector>

class Player {
public:
    void init(Graphics* graphics);
    void destroy();
    void reset();
    
    void handleEvent(SDL_Event& event);
    void update(const float& deltatime, const bool& invadersReachedBarriers, std::vector<Bullet>& bullets, const int& nKills);
    void move(const int& x);
    
    int  lives() { return livesRemaining; }
    void render();

    int  score() { return currentScore; }
    bool isDead() { return isPlayerDead; }
private:
    bool wasShot(std::vector<Bullet>& bullets);
    void handleDeath(const float& deltatime);
    void move(const float& deltatime);
    
    Graphics* graphics;
    
    int  currentScore       { 0 };
    bool shoot              { false };
    
    int   livesRemaining    { 3 };
    int   deathTicks        { 0 };
    float deathCounter      { 0.0f };
    bool  renderPlayer      { true };
    bool  isPlayerDead      { false };
    
    const int PLAYER_SPEED  { 300 };
    int  direction          { dir::stop };
    enum dir    { stop = 0, left = -1, right = 1};
    
    const int WINDOW_WIDTH  { 434 };
    const int WINDOW_HEIGHT { 496 };
    const int WINDOW_BORDER { 10 };
    
    const int SPRITE_WIDTH  { 26 };
    const int SPRITE_HEIGHT { 16 };
    const int DEFAULT_XPOS  { (WINDOW_WIDTH / 2) - SPRITE_WIDTH / 2 };
    const int DEFAULT_YPOS  { WINDOW_HEIGHT - 80 };
    
    SDL_Texture* playerTexture { nullptr };
    SDL_Rect playerPosition;
    
    Mix_Chunk* playerDied   { nullptr };
    Mix_Chunk* gameOverSound{ nullptr };
    Mix_Chunk* playerLaser  { nullptr };
};

#endif
