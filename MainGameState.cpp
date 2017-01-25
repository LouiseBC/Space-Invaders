#include "MainGameState.hpp"
#include "Graphics.hpp"

bool MainGameState::init(Graphics *graphs, Game *gam) {
    graphics = graphs;
    game = gam;
    
    player.init(graphics);
    enemyhandler.init(graphics);
    barrier1.init(graphics, BARRIER_1_X);
    barrier2.init(graphics, BARRIER_2_X);
    barrier3.init(graphics, BARRIER_3_X);
    barrier4.init(graphics, BARRIER_4_X);
    hud.init(graphics);
    
    bullets.push_back(Bullet(Bullet::direction::up));
    for (int i = 0; i < 55; ++i) // 55 invaders in total
        bullets.push_back(Bullet(Bullet::direction::down));
    
    return true;
}

MainGameState::~MainGameState() {
    Mix_Quit();
    player.destroy();
    enemyhandler.destroy();
}

void MainGameState::restart() {
    player.reset();
    enemyhandler.reset();
    barrier1.reset();
    barrier2.reset();
    barrier3.reset();
    barrier4.reset();
    for (int i = bullets.size(); i >= 0; --i)
        bullets[i].reset();
    
    pause = false;
    gameOver = false;
}

void MainGameState::handleEvents(SDL_Event &event) {
    if (event.type == SDL_QUIT)
        game->setQuit();
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        game->setQuit();
    else if (event.type == SDL_KEYDOWN && player.lives() == 0)
        restart();
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
            pause = !pause;
    else
        player.handleEvent(event);
        
}

void MainGameState::update(const float &deltatime) {
    if (!(player.lives() == 0)) {
        if (player.isDead())
            // only update player so it can recover itself
            player.update(deltatime, enemyhandler.haveReachedBarriers(), bullets, enemyhandler.killCount());
        else if (!pause) {
            // update everything
            player.update(deltatime, enemyhandler.haveReachedBarriers(), bullets, enemyhandler.killCount());
            enemyhandler.update(deltatime, bullets);
            barrier1.update(bullets);
            barrier2.update(bullets);
            barrier3.update(bullets);
            barrier4.update(bullets);
            for (int i = bullets.size(); i >= 0; --i)
                bullets[i].update(deltatime);
        }
    }
}

void MainGameState::render() {
    graphics->renderClear();
    
    player.render();
    enemyhandler.render();
    barrier1.render();
    barrier2.render();
    barrier3.render();
    barrier4.render();
    
    SDL_SetRenderDrawColor(graphics->renderer(), 255, 255, 255, 1);
    for (int i = bullets.size(); i >= 0; --i) {
        if (bullets[i].isOnScreen()) {
            SDL_Rect temp = bullets[i].position();
            SDL_RenderFillRect(graphics->renderer(), &temp);
        }
    }
        
    hud.render();
    hud.renderScore(player.score());
    hud.renderLives(player.lives());
    if (pause)
        hud.renderPause();
    if (player.lives() == 0)
        hud.renderGameOver();
    
    graphics->renderPresent();
}