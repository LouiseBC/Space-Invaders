// The main Graphics class, provides a renderer and a window
// (Also handles sound effects)

#ifndef Graphics_hpp
#define Graphics_hpp
#include <SDL2/SDL.h>
#include <string>
#include <SDL2_mixer/SDL_mixer.h>

class Graphics {
public:
    bool setup();
    void destroy();
    
    SDL_Renderer* renderer() { return globalRenderer; }
    void renderClear() { SDL_SetRenderDrawColor(globalRenderer, 50, 50, 50, 1); SDL_RenderClear(globalRenderer); }
    void renderPresent() { SDL_RenderPresent(globalRenderer); }
    
    SDL_Texture* loadTexture(std::string filepath);
    Mix_Chunk* loadSound(std::string filepath);
    
    SDL_Texture* renderText(const std::string &message, const std::string &filepath, SDL_Color colour, int fontsize);
    
    int screenWidth() const { return WINDOW_WIDTH; }
    int screenHeight() const { return WINDOW_HEIGHT; }
    
private:
    SDL_Window* globalWindow      { nullptr };
    SDL_Renderer* globalRenderer  { nullptr };
    
    const int WINDOW_WIDTH        { 434 };
    const int WINDOW_HEIGHT       { 496 };
};

#endif
