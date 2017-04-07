#include "Player.hpp"
#include <iostream>

void Player::init(Graphics *graphs) {
    graphics = graphs;
    
    playerPosition = (SDL_Rect{DEFAULT_XPOS, DEFAULT_YPOS, SPRITE_WIDTH, SPRITE_HEIGHT});
    playerTexture = graphics->loadTexture("Textures/player.png");
    playerDied = graphics->loadSound("Sounds/explosion.wav");
    playerLaser = graphics->loadSound("Sounds/shoot.wav");
    gameOverSound = graphics->loadSound("Sound/superdead.wav");
    
    Mix_VolumeChunk(playerLaser, MIX_MAX_VOLUME/2);
}

void Player::destroy() {
    Mix_FreeChunk(playerDied);
    SDL_DestroyTexture(playerTexture);
}

void Player::reset() {
    playerPosition.x = DEFAULT_XPOS;
    playerPosition.y = DEFAULT_YPOS;
    direction = dir::stop;
    isPlayerDead = false;
    currentScore = 0;
    livesRemaining = 3;
}

void Player::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT)
            direction = dir::left;
        else if (event.key.keysym.sym == SDLK_RIGHT)
            direction = dir::right;
        else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
            shoot = true;
    }

    else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LEFT)
            direction = dir::stop;
        else if (event.key.keysym.sym == SDLK_RIGHT)
            direction = dir::stop;
    }
}

void Player::update(const float& deltatime, const bool& invadersReachedBarriers, std::vector<Bullet>& bullets, const int& nKills) {
    if (isPlayerDead)
        handleDeath(deltatime);
    
    else if (invadersReachedBarriers) {
        //Mix_HaltChannel(1); // stop spaceship in case it's playing
        Mix_PlayChannel(-1, playerDied, 0);
        isPlayerDead = true;
        livesRemaining = 0;
    }
    else if (wasShot(bullets)) {
        //Mix_HaltChannel(1); // ?
        Mix_PlayChannel(-1, playerDied, 0);
        isPlayerDead = true;
    }
    
    else {
        if (direction != dir::stop)
            move(deltatime);
        
        if (shoot) {
            if (!bullets[0].isOnScreen()) {
                bullets[0].setPosition(playerPosition.x + SPRITE_WIDTH/2 - 1, playerPosition.y);
                Mix_PlayChannel(-1, playerLaser, 0);
            }
            shoot = false;
        }
    }
    
    if (currentScore != nKills * 10)
        currentScore = nKills * 10;
}

void Player::handleDeath(const float& deltatime) {
    if (livesRemaining == 1) {
        Mix_PlayChannel(-1, gameOverSound, 0);
        --livesRemaining;
    }
    else {
        if (deathCounter >= 0.2f && deathTicks < 6) {
            renderPlayer = !renderPlayer;
            ++deathTicks;
            deathCounter = 0.0f;
        }
        else if (deathCounter >= 0.2f && deathTicks == 6) {
            deathCounter = 0.0f;
            deathTicks = 0;
            --livesRemaining;
            if (livesRemaining >= 1) {
                isPlayerDead = false;
                playerPosition.x = DEFAULT_XPOS;
                shoot = false;
            }
        }
        else deathCounter += deltatime;
    }
}

void Player::move(const float &deltatime) {
    if (playerPosition.x + (int)(direction * PLAYER_SPEED * deltatime) >= WINDOW_BORDER &&
        playerPosition.x + playerPosition.w + (int)(direction * PLAYER_SPEED * deltatime) <= graphics->screenWidth() - WINDOW_BORDER)
        playerPosition.x += (int)(direction * PLAYER_SPEED * deltatime);
}

void Player::move(const int& x) {
    playerPosition.x = x;
}

bool Player::wasShot(std::vector<Bullet>& bullets) {
    for (int i = bullets.size()-1; i >= 1; --i) {
        if (bullets[i].isOnScreen()) {
            if (!(bullets[i].position().y > playerPosition.y + playerPosition.h ||
                  bullets[i].position().y + bullets[i].position().h < playerPosition.y ||
                  bullets[i].position().x < playerPosition.x ||
                  bullets[i].position().x + bullets[i].position().w > playerPosition.x + playerPosition.w)){
                bullets[i].reset();
                return true;
            }
        }
    }
    return false;
}

void Player::render() {
    if (renderPlayer)
        SDL_RenderCopy(graphics->renderer(), playerTexture, NULL, &playerPosition);
}