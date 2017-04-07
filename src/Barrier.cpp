#include "Barrier.hpp"
#include "Graphics.hpp"

void Barrier::init(Graphics *graphs, const int& X) {
    graphics = graphs;
    
    int y = BARRIER_Y;
    for (int i = 0; i < 3; ++i) {
        int x = X;
        for (int j = 0; j < 4; ++j) {
            if (!(i == 2 && (j == 1 || j == 2))) { // don't draw middle blocks in last row
                blocks.push_back(BarrierBlock{SDL_Rect{ x, y, BLOCK_WIDTH, BLOCK_HEIGHT}});
            }
            x += BLOCK_WIDTH;
        }
        y += BLOCK_HEIGHT;
    }
}

void Barrier::reset() {
    for (int i = blocks.size()-1; i >= 0; --i) {
        blocks[i].alpha = 255;
        blocks[i].dead = false;
    }
}

void Barrier::damage(const int& blocknum) {
    blocks[blocknum].alpha -= ALPHAMOD;
}

void Barrier::update(std::vector<Bullet> &bullets) {
    handleCollision(bullets);
    
    for (int i = blocks.size()-1; i >= 0; --i) {
        if (blocks[i].alpha == (255 - 4*ALPHAMOD)) // dead after 4th blast
            blocks[i].dead = true;
    }
}

void Barrier::handleCollision(std::vector<Bullet> &bullets) {
    for (int i = blocks.size()-1; i >= 0; --i) {
        if (!blocks[i].dead) {
            for (int j = bullets.size()-1; j >= 0; --j) {
                if (!(bullets[j].position().y > blocks[i].position.y + blocks[i].position.h ||
                      bullets[j].position().y + bullets[j].position().h < blocks[i].position.y ||
                      bullets[j].position().x < blocks[i].position.x ||
                      bullets[j].position().x + bullets[j].position().w > blocks[i].position.x + blocks[i].position.w)) {
                    damage(i);
                    bullets[j].reset();
                }
            }
        }
    }
}

void Barrier::render() {
    SDL_SetRenderDrawColor(graphics->renderer(), 41, 250, 46, 1);
    
    for (int i = blocks.size()-1; i >= 0; --i) {
        if (blocks[i].dead == false) {
            if (blocks[i].alpha < 255) {
                SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(graphics->renderer(), 41, 250, 46, blocks[i].alpha);
                SDL_RenderFillRect(graphics->renderer(), &blocks[i].position);
                SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_NONE);
            }
            else
                SDL_RenderFillRect(graphics->renderer(), &blocks[i].position);
        }
    }
}