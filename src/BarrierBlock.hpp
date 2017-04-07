#ifndef BarrierBlock_hpp
#define BarrierBlock_hpp
#include <SDL2/SDL.h>

struct BarrierBlock {
    BarrierBlock(SDL_Rect blockposition) :
    position {blockposition} {}
    
    int alpha { 255 };
    SDL_Rect position;
    bool dead { false };
};

#endif