#ifndef MainGameState_hpp
#define MainGameState_hpp
#include "GameState.hpp"
#include "Player.hpp"
#include "EnemyHandler.hpp"
#include "Barrier.hpp"
#include "HUD.hpp"
#include "Bullet.hpp"
#include <vector>

class MainGameState : public GameState {
public:
    bool init(Graphics* graphics, Game* game);
    ~MainGameState();
    
    void restart();
    void handleEvents(SDL_Event& event);
    void update(const float& deltatime);
    void render();
    
private:
    Graphics* graphics { nullptr };
    Game* game         { nullptr };

    bool gameOver      { false };
    bool pause         { false };
    
    Player player;
    EnemyHandler enemyhandler;
    Barrier barrier1;
    Barrier barrier2;
    Barrier barrier3;
    Barrier barrier4;
    HUD hud;
    
    const int BARRIER_1_X { 60 };
    const int BARRIER_2_X { 150 };
    const int BARRIER_3_X { 240 };
    const int BARRIER_4_X { 330 };
    
    std::vector<Bullet> bullets;
};

#endif
