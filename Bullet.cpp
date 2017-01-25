#include "Bullet.hpp"

void Bullet::update(const float& deltatime) {
    if (isOnScreen())
        bulletPos.y += deltatime * bulletSpeed * DIRECTION;
    else reset();
}

void Bullet::reset() {
    bulletPos.y = WINDOW_HEIGHT + 1;
}

bool Bullet::isOnScreen() {
    return !(bulletPos.y > WINDOW_HEIGHT || bulletPos.y + bulletPos.h < 0);
}

void Bullet::setPosition(int x, int y) {
    if (DIRECTION == direction::up)
        bulletPos = SDL_Rect{x, y - HEIGHT, WIDTH, HEIGHT};
    else bulletPos = SDL_Rect{x, y + HEIGHT, WIDTH, HEIGHT};
}