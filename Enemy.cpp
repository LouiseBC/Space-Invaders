#include "Enemy.hpp"

void Enemy::move() {
    updateCurrentClip();
    currentPos.x += direction * speed;
}

void Enemy::changeDirection() {
    updateCurrentClip();
    currentPos.y += speed; // move down
    direction = direction * -1; // reverse direction
}

void Enemy::updateCurrentClip() {
    if (currentFrame == &clip1 )
        currentFrame = &clip2;
    else currentFrame = &clip1;
}

void SpaceShip::move(const float &deltatime) {
    currentPos.x += direction * speed * deltatime;
    
    if (jiggleCount >= 0.05f) {
        if (jiggleAmount == 0)
            jiggleDirection = 1;
        else if (jiggleAmount == 4)
            jiggleDirection = -1;
        
        currentPos.y += 1 * jiggleDirection;
        jiggleAmount += 1 * jiggleDirection;
        jiggleCount = 0;
    }
    else jiggleCount += deltatime;
}