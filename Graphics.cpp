#include "Graphics.hpp"
#include <iostream>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>

bool Graphics::setup() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "Error: init" << SDL_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() < 0) {
        std::cerr << "Error: TTF init" << TTF_GetError << std::endl;
        return false;
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Error: OpenAudio Init" << Mix_GetError() << std::endl;
        return false;
    }

    globalWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (globalWindow == nullptr) {
        std::cerr << "Error: Create window" << SDL_GetError() << std::endl;
        return false;
    }
    
    
    globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (globalRenderer == nullptr) {
        std::cerr << "Error: Create renderer" << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void Graphics::destroy() {
    TTF_Quit();
    SDL_DestroyRenderer(globalRenderer);
    SDL_DestroyWindow(globalWindow);
}

SDL_Texture* Graphics::loadTexture(std::string filepath) {
    SDL_Texture* temp = IMG_LoadTexture(globalRenderer, filepath.c_str());
    if (temp == nullptr)
        std::cerr << "Error: Load texture " << filepath << std::endl;
    return temp;
}

Mix_Chunk* Graphics::loadSound(std::string filepath) {
    Mix_Chunk* temp = Mix_LoadWAV(filepath.c_str());
    if( temp == nullptr )
        std::cerr << "Failed to load sound', error:" << Mix_GetError() << std::endl;
    return temp;
}

SDL_Texture* Graphics::renderText(const std::string &message, const std::string &filepath, SDL_Color colour, int fontsize) {
    TTF_Font *font = TTF_OpenFont(filepath.c_str(), fontsize);
    if (font == nullptr){
        std::cerr << "Error: TTF_OpenFont " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Surface *surf = TTF_RenderText_Solid(font, message.c_str(), colour);
    if (surf == nullptr){
        TTF_CloseFont(font);
        std::cout << "Error: TTF_RenderText " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(globalRenderer, surf);
    if (texture == nullptr)
        std::cout << "Error: CreateTexture " << SDL_GetError() << std::endl;
    
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}