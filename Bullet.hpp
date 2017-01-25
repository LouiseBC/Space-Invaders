#ifndef Bullet_hpp
#define Bullet_hpp
#include "SDL2/SDL.h"

class Bullet {
public:
    Bullet(int dir) :
    DIRECTION { dir } { if (dir == direction::down) bulletSpeed = 300; }
    
    void setPosition(int x, int y);
    SDL_Rect position() { return bulletPos; }
    
    void update(const float& deltatime);
    bool isOnScreen();
    void reset();
    
    enum direction { up = -1, down = 1 };
private:
    const int DIRECTION;
    const int WINDOW_HEIGHT { 496 };
    int       bulletSpeed   { 500 };
    
    const int HEIGHT { 8 };
    const int WIDTH  { 2 };
    
    SDL_Rect bulletPos { 0, WINDOW_HEIGHT + 1, WIDTH, HEIGHT };
};

#endif
