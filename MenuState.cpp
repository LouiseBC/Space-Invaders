#include "MenuState.hpp"
#include "MainGameState.hpp"

bool MenuState::init(Graphics *graphs, Game *gam) {
    graphics = graphs;
    game = gam;
    
    title = graphics->renderText("SPACE INVADERS", "Fonts/space_invaders.ttf", greenColour, 35);
    title2 = graphics->renderText("SPACE INVADERS", "Fonts/space_invaders.ttf", whiteColour, 35);
    startInstr = graphics->renderText("Press any key to start", "Fonts/space_invaders.ttf", greenColour, 17);
    controls = graphics->renderText("Controls: Arrow keys", "Fonts/space_invaders.ttf", whiteColour, 12);
    controls2 = graphics->renderText("Press space to pause", "Fonts/space_invaders.ttf", whiteColour, 12);
    
    int W; int H;
    // Render main title
    SDL_QueryTexture(title, NULL, NULL, &W, &H);
    titleRect = SDL_Rect{ graphics->screenWidth()/2 -  W/2, graphics->screenHeight()/2 - 2*H, W, H };
    titleRect2 = titleRect;
    titleRect2.y += 1; titleRect2.x += 1;
    titleRect3 = titleRect2;
    titleRect3.y += 1; titleRect3.x += 1;
    
    // Render 'press x to start'
    SDL_QueryTexture(startInstr, NULL, NULL, &W, &H);
    startInstrRect = SDL_Rect{ graphics->screenWidth()/2 - W/2, titleRect.y + titleRect.h + 10, W, H };
    
    // Render Controls
    SDL_QueryTexture(controls, NULL, NULL, &W, &H);
    controlsRect = SDL_Rect{ 10, graphics->screenHeight() - 2* H, W, H };
    SDL_QueryTexture(controls2, NULL, NULL, &W, &H);
    controls2Rect = SDL_Rect{ graphics->screenWidth() - W - 10, controlsRect.y, W, H };
    
    return true;
}

MenuState::~MenuState() {
    
}

void MenuState::handleEvents(SDL_Event &event) {
    if (event.type == SDL_QUIT)
        game->setQuit();
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        game->setQuit();
    else if (event.type == SDL_KEYDOWN)
        game->changeState(new MainGameState);
}

void MenuState::update(const float &deltatime) {
    
}

void MenuState::render() {
    graphics->renderClear();
    
    renderTitle();
    renderControls();
    
    graphics->renderPresent();
}

void MenuState::renderTitle() {
    SDL_RenderCopy(graphics->renderer(), title2, NULL, &titleRect3);
    SDL_RenderCopy(graphics->renderer(), title2, NULL, &titleRect2);
    SDL_RenderCopy(graphics->renderer(), title, NULL, &titleRect);
    SDL_RenderCopy(graphics->renderer(), startInstr, NULL, &startInstrRect);
}

void MenuState::renderControls() {
   SDL_RenderCopy(graphics->renderer(), controls, NULL, &controlsRect);
   SDL_RenderCopy(graphics->renderer(), controls2, NULL, &controls2Rect);
}