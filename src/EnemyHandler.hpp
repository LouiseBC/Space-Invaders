#ifndef Enemies_hpp
#define Enemies_hpp
#include "Graphics.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include <vector>
#include <random>

class EnemyHandler {
public:
    void init(Graphics* graphics);
    void destroy();
    void reset();
    void prepareNewWave(const float& deltatime, std::vector<Bullet>& bullets);
    
    void update(const float& deltatime, std::vector<Bullet>& bullets);
    bool haveReachedBarriers();
    void render();
    int  killCount() { return killScore; }
private:
    void maybeSpawnSpaceShip(const float& deltatime);
    void moveInvaders(const float& deltatime);
    void moveSpaceShip(const float& deltatime);
    bool haveReachedWindowBorder();
    
    void handleBulletCollision(std::vector<Bullet>& bullets);
    bool collidedWithBullet(Bullet& playerbullet, SDL_Rect* enemyposition);
    
    void maybeShoot(std::vector<Bullet>& bullets);
    void handleShooting(Enemy* shooter, Bullet& bullet);
    
    void setEnemyPositions();
    void playBeepSound();
    
    // Data members
    ////////////////
    Graphics* graphics;
    SDL_Texture* spriteSheet;
    std::vector<Enemy*> enemies;
    SpaceShip* spaceship { nullptr };
    
    // Enemy positioning data
    const int SIDE_MARGIN         { 45 };
    const int TOP_MARGIN          { 128 };
    const int ENEMY_SPACING       { 8 };
    const int ENEMY_ROW_SPACING   { 16 };
    
    // Enemy movement borders
    const int BARRIER_Y           { 368 };
    const int WINDOW_BORDER       { 15 };
    bool enemyReachedWindowBorder { false };
    
    // Enemy movement timing data
    const float DEFAULT_CAP { 0.8f};
    float time              { 0.0f };
    float timeCap           { DEFAULT_CAP };
    float timeSinceShip     { 3.0f };
    
    float waveTimer         { 0.0f };
    int   renderCap         { 0 };
    
    int   killScore         { 0 };
    int   remainingInvaders { 0 };
    
    // Sound effects
    Mix_Chunk* ufo   { nullptr };
    Mix_Chunk* death { nullptr };
    Mix_Chunk* shoot { nullptr };
    Mix_Chunk* beep1 { nullptr };
    Mix_Chunk* beep2 { nullptr };
    Mix_Chunk* beep3 { nullptr };
    Mix_Chunk* beep4 { nullptr };
    int currentBeep  { 1 };
};

#endif
