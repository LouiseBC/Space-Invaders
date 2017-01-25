#ifndef enemy_h
#define enemy_h
#include <SDL2/SDL.h>

class Enemy {
public:
    void move();
    int  currDirection() { return direction; }
    void changeDirection();
    
    bool isDead() { return isEnemyDead; }
    void setDead()  { isEnemyDead = true; }
    int  type() { return enemyType; }
    enum type { low = 0, med, high, spaceship};
    
    // Rendering-related functions
    int  width()         { return WIDTH; }
    int  height()        { return HEIGHT; }
    int  widthOffset()   { return WIDTHDIFFERENCE; }
    SDL_Rect* position() { return &currentPos; }
    SDL_Rect* currentClip() const { return currentFrame; }
protected:
    void updateCurrentClip();
    
    bool isEnemyDead    { false };
    int enemyType;
    
    int direction       { 1 };
    int speed           { 4 };
    
    int HEIGHT          { 16 };
    int WIDTH;
    int WIDTHDIFFERENCE;
    
    SDL_Rect currentPos;
    SDL_Rect clip1;
    SDL_Rect clip2;
    SDL_Rect* currentFrame { &clip1 };
};

class LowEnemy : public Enemy {
public:
    LowEnemy(int X, int Y) {
        enemyType = type::low;
        WIDTH = 24;
        WIDTHDIFFERENCE = 0;
        currentPos = SDL_Rect{ X + WIDTHDIFFERENCE/2, Y, WIDTH, HEIGHT };
        clip1 = SDL_Rect{ 0, 74, WIDTH*2, HEIGHT*2 };
        clip2 = SDL_Rect{ 52, 74, WIDTH*2, HEIGHT*2 };
    }
};

class MedEnemy : public Enemy {
public:
    MedEnemy(int X, int Y) {
        enemyType = type::med;
        WIDTH = 22;
        WIDTHDIFFERENCE = 2;
        currentPos = SDL_Rect{ X + WIDTHDIFFERENCE/2, Y, WIDTH, HEIGHT };
        clip1 = SDL_Rect{ 0, 37, WIDTH*2, HEIGHT*2 };
        clip2 = SDL_Rect{ 49, 37, WIDTH*2, HEIGHT*2 };
    }
};

class HighEnemy : public Enemy {
public:
    HighEnemy(int X, int Y) {
        enemyType = type::high;
        WIDTH = 16;
        WIDTHDIFFERENCE = 8;
        currentPos = SDL_Rect{ X + WIDTHDIFFERENCE/2, Y, WIDTH, HEIGHT };
        clip1 = SDL_Rect{ 0, 0, WIDTH*2, HEIGHT*2 };
        clip2 = SDL_Rect{ 36, 0, WIDTH*2, HEIGHT*2 };
    }
};

class SpaceShip : public Enemy {
public:
    SpaceShip() {
        enemyType = type::spaceship;
        speed = 150; // spaceship moves in pixels per second
        WIDTH = 45;
        HEIGHT = 21;
        currentPos = SDL_Rect{ 0 - WIDTH, 4* HEIGHT, WIDTH, HEIGHT };
        clip1 = SDL_Rect{ 0, 112, WIDTH*2, HEIGHT*2 };
        clip2 = clip1;
    }
    
    void move(const float& deltatime);
private:
    int jiggleAmount    { 0 };
    int jiggleDirection { 1 };
    float jiggleCount   { 0.0f};
};


#endif