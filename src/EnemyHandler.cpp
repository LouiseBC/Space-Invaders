#include "EnemyHandler.hpp"
#include <iostream>
#include <random>

int getRandomInt(const int& lowerbound, const int& upperbound) {
    std::random_device random;
    std::mt19937 rng(random());
    std::uniform_int_distribution<int> range(lowerbound, upperbound);
    return range(random);
}

void EnemyHandler::init(Graphics* graphs) {
    graphics = graphs;
    spriteSheet = graphics->loadTexture("Textures/spritesheet.png");
    
    beep1 = graphics->loadSound("Sounds/beep4.wav");
    beep2 = graphics->loadSound("Sounds/beep1.wav");
    beep3 = graphics->loadSound("Sounds/beep2.wav");
    beep4 = graphics->loadSound("Sounds/beep3.wav");
    ufo   = graphics->loadSound("Sounds/ufo.wav");
    death = graphics->loadSound("Sounds/invaderkilled.wav");
    shoot = graphics->loadSound("Sounds/invadershoot.wav");
    Mix_VolumeChunk(ufo, MIX_MAX_VOLUME/2);
}

void EnemyHandler::destroy() {
    Mix_FreeChunk(beep1);
    Mix_FreeChunk(beep2);
    Mix_FreeChunk(beep3);
    Mix_FreeChunk(beep4);
    Mix_FreeChunk(ufo);
    Mix_FreeChunk(death);
    SDL_DestroyTexture(spriteSheet);
}

void EnemyHandler::reset() {
    // Clear enemies vector
    if (enemies.size() > 0 ) {
    for (int i = enemies.size()-1; i >= 0; --i)
        delete enemies[i];
    enemies.clear();
    }
    // Load new enemies
    setEnemyPositions();
    timeCap = DEFAULT_CAP;
    currentBeep = 1;
    remainingInvaders = enemies.size();
    enemyReachedWindowBorder = false;
    killScore = 0;
    
    // Reset spaceship
    timeSinceShip = 3.0f;
    if (spaceship != nullptr) {
        delete spaceship;
        spaceship = nullptr;
        Mix_HaltChannel(1);
    }
}

void EnemyHandler::setEnemyPositions() {
    int x = SIDE_MARGIN;
    int y = TOP_MARGIN;
    // Push back highest row
    for (int j = 0; j < 11; ++j) {
        enemies.push_back(new HighEnemy(x, y));
        x += enemies.back()->width() + ENEMY_SPACING + enemies.back()->widthOffset();
    }
    // Push back 3 middle rows
    y += enemies.back()->height() + ENEMY_ROW_SPACING;
    for (int i = 0; i < 3; ++i) {
        x = SIDE_MARGIN;
        for (int j = 0; j < 11; ++j) {
            enemies.push_back(new MedEnemy(x, y));
            x += enemies.back()->width() + ENEMY_SPACING + enemies.back()->widthOffset();
        }
        y += enemies.back()->height() + ENEMY_ROW_SPACING;
    }
    // Push back lowest row
    x = SIDE_MARGIN;
    for (int j = 0; j < 11; ++j) {
        enemies.push_back(new LowEnemy(x, y));
        x += enemies.back()->width() + ENEMY_SPACING + enemies.back()->widthOffset();
    }
}

void EnemyHandler::update(const float& deltatime, std::vector<Bullet>& bullets) {
    if (remainingInvaders == 0)
        prepareNewWave(deltatime, bullets);
    else {
        handleBulletCollision(bullets);
        moveInvaders(deltatime);
        maybeShoot(bullets);
        
        if (spaceship == nullptr)
            maybeSpawnSpaceShip(deltatime);
        else
            moveSpaceShip(deltatime);
    }
}

void EnemyHandler::prepareNewWave(const float& deltatime, std::vector<Bullet>& bullets) {
    if (waveTimer == 0.0f) {
        waveTimer += deltatime;
        for (int i = bullets.size(); i >= 0; --i) {
            if (bullets[i].isOnScreen())
                bullets[i].reset();
        }
        reset();
        remainingInvaders = 0; // Still want it at 0 for now
        renderCap = enemies.size();
    }
    else if (waveTimer >= 1.0f && renderCap == 11) {
        renderCap = 0;
        waveTimer = 0.0f;
        remainingInvaders = enemies.size(); // break out of loop;
        return;
    }
    else if (waveTimer >= 0.8f && renderCap == 22)
        renderCap = 11;
    else if (waveTimer >= 0.6f && renderCap == 33)
        renderCap = 22;
    else if (waveTimer >= 0.4f && renderCap == 44)
        renderCap = 33;
    else if (waveTimer >= 0.2f && renderCap == enemies.size())
        renderCap = 44;
    waveTimer += deltatime;
}

void EnemyHandler::handleBulletCollision(std::vector<Bullet>& bullets) {
    // If player's bullet is active
    if (bullets[0].isOnScreen()) {
        // Check if it killed any invaders
        for (int i = enemies.size()-1; i >= 0; --i) {
            if (!enemies[i]->isDead()) {
                if (collidedWithBullet(bullets[0], enemies[i]->position())) {
                    enemies[i]->setDead();
                    killScore += enemies[i]->type() +1; // count starts at 0
                    --remainingInvaders;
                    bullets[0].reset();
                    Mix_PlayChannel(-1, death, 0);
                }
            }
        }
        // Check if it killed a spaceship
        if (spaceship != nullptr) {
            if (collidedWithBullet(bullets[0], spaceship->position())) {
                delete spaceship;
                spaceship = nullptr;
                bullets[0].reset();
                Mix_HaltChannel(1);
                Mix_PlayChannel(-1, death, 0);
                int rand = getRandomInt(1, 4);
                switch(rand) {
                    case 1: killScore += 5; break;
                    case 2: killScore += 10; break;
                    case 3: killScore += 15; break;
                    case 4: killScore += 30; break;
                }
            }
        }
    }
}

bool EnemyHandler::collidedWithBullet(Bullet& playerbullet, SDL_Rect* enemyposition) {
    return (!(playerbullet.position().y >= enemyposition->y + enemyposition->h ||
              playerbullet.position().y >= enemyposition->y + enemyposition->h/4 ||
              playerbullet.position().x <= enemyposition->x ||
              playerbullet.position().x + playerbullet.position().w >= enemyposition->x + enemyposition->w));
}

void EnemyHandler::moveInvaders(const float &deltatime) {
    if (time >= timeCap)
    {
        time = 0.0f;
        playBeepSound();
        enemyReachedWindowBorder = haveReachedWindowBorder();
        
        // Move invaders
        for (int i = enemies.size()-1; i >= 0; --i) {
            if (!enemies[i]->isDead()) {
                if (enemyReachedWindowBorder)
                    enemies[i]->changeDirection();
                else enemies[i]->move();
            }
        }
        // increase invader speed
        if (enemyReachedWindowBorder) {
            if (timeCap >= 0.3f)
                timeCap -= 0.1f;
        }
    }
    else time += deltatime;
}

bool EnemyHandler::haveReachedWindowBorder() {
    // Find a live enemy
    int alive = -1;
    for (int i = enemies.size()-1; i >= 0; --i) {
        if (!enemies[i]->isDead()) {
            alive = i;
            break;
        }
    }
    // Checks if rightmost invaders reached border
    if (enemies[alive]->currDirection() == 1) {
        for (int i = enemies.size()-1; i >= 0; --i) {
            if (!enemies[i]->isDead() &&
                enemies[i]->position()->x + enemies[i]->position()->w >= graphics->screenWidth() - WINDOW_BORDER)
                return true;
        }
    }
    // Checks if leftmost invaders reached border
    else if (enemies[alive]->currDirection() == -1) {
        for (int i = enemies.size()-1; i >= 0; --i) {
            if (!enemies[i]->isDead() &&
                enemies[i]->position()->x <= WINDOW_BORDER)
                return true;
        }
    }
    return false;
}

void EnemyHandler::maybeShoot(std::vector<Bullet>& bullets) {
    // Find lowest enemy in each of 5 columns
    for (int i = enemies.size()-1; i >= 44; --i) {
        for (int j = 0; j <= 44; j += 11) {
            if (!enemies[i-j]->isDead() && !bullets[i-j+1].isOnScreen()) {
                handleShooting(enemies[i-j], bullets[i-j+1]);
                break;
            }
        }
    }
}

void EnemyHandler::handleShooting(Enemy* shooter, Bullet& bullet) {
    int randomInt = getRandomInt(1, 1000);
    if (randomInt == 1) {
        bullet.setPosition(shooter->position()->x + shooter->position()->w/2, shooter->position()->y + shooter->position()->h);
        Mix_PlayChannel(-1, shoot, 0);
    }
}

void EnemyHandler::moveSpaceShip(const float &deltatime) {
    if (spaceship->position()->x == 0 - spaceship->width())
        Mix_PlayChannel(1, ufo, -1);
    
    spaceship->move(deltatime);
    if (spaceship->position()->x > graphics->screenWidth()) {
        delete spaceship;
        spaceship = nullptr;
        Mix_HaltChannel(1);
    }
}

bool EnemyHandler::haveReachedBarriers() {
    for (int i = enemies.size()-1; i >= 0; --i) {
        if (!enemies[i]->isDead() &&
            enemies[i]->position()->y + enemies[i]->position()->h > BARRIER_Y)
            return true;
    }
    return false;
}

void EnemyHandler::maybeSpawnSpaceShip(const float& deltatime) {
    if (spaceship == nullptr) {
        timeSinceShip += deltatime;
    
        int randomInt = getRandomInt(1, 1000);
        
        if (randomInt == 1 && timeSinceShip >= 4.0f) {
            spaceship = new SpaceShip;
            timeSinceShip = 0.0f;
        }
    }
}

void EnemyHandler::playBeepSound() {
    switch(currentBeep) {
        case 1: Mix_PlayChannel(-1, beep1, 0);
            break;
        case 2: Mix_PlayChannel(-1, beep2, 0);
            break;
        case 3: Mix_PlayChannel(-1, beep3, 0);
            break;
        case 4: Mix_PlayChannel(-1, beep4, 0);
            break;
    }
    if (currentBeep <= 3)
        ++currentBeep;
    else currentBeep = 1;
}

void EnemyHandler::render() {
    for (int i = enemies.size()-1; i >= renderCap; --i) {
        if (!enemies[i]->isDead())
            SDL_RenderCopy(graphics->renderer(), spriteSheet, enemies[i]->currentClip(), enemies[i]->position());
    }
    
    if (spaceship != nullptr)
        SDL_RenderCopy(graphics->renderer(), spriteSheet, spaceship->currentClip(), spaceship->position());
}