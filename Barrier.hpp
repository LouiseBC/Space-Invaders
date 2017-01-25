#ifndef Barrier_hpp
#define Barrier_hpp
#include <vector>
#include "BarrierBlock.hpp"
#include "Bullet.hpp"

class Graphics;

class Barrier {
public:
    void init(Graphics* graphics, const int& X);
    void reset();
    
    void update(std::vector<Bullet> &bullets);
    void handleCollision(std::vector<Bullet> &bullets);
    void damage(const int& blocknum);
    void render();
private:
    Graphics* graphics;
    
    const int BLOCK_WIDTH { 11 };
    const int BLOCK_HEIGHT { 11 };
    const int BARRIER_Y { 368 };
    const int ALPHAMOD { 65 };
    
    std::vector<BarrierBlock> blocks;
};

#endif
