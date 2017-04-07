#include "HUD.hpp"
#include "Graphics.hpp"

void HUD::init(Graphics *graphs) {
    graphics = graphs;
    
    score = graphics->renderText("Score <1> :", "Fonts/space_invaders.ttf", fontColour, 20);
    currentScore = graphics->renderText("0", "Fonts/space_invaders.ttf", greenColour, 20);
    
    lives = graphics->renderText("Lives:", "Fonts/space_invaders.ttf", fontColour, 20);
    
    gameOver = graphics->renderText("GAME OVER", "Fonts/space_invaders.ttf", greenColour, 50);
    restartInstr = graphics->renderText("Press space to restart", "Fonts/space_invaders.ttf", greenColour, 20);
    pause = graphics->renderText("-Paused-", "Fonts/space_invaders.ttf", fontColour, 20);
    
    int W; int H;
    SDL_QueryTexture(score, NULL, NULL, &W, &H);
    scoreRect = SDL_Rect{15, 5, W, H};
    SDL_QueryTexture(currentScore, NULL, NULL, &W, &H);
    currentScoreRect = SDL_Rect{scoreRect.x + scoreRect.w + 5, scoreRect.y, W, H};
    
    SDL_QueryTexture(lives, NULL, NULL, &W, &H);
    livesRect = SDL_Rect{graphics->screenWidth() - (int)(1.5 * W), 5, W, H};
    
    SDL_QueryTexture(gameOver, NULL, NULL, &W, &H);
    gameOverRect = SDL_Rect{graphics->screenWidth()/2 - W/2, graphics->screenHeight()/2 - H, W, H};
    SDL_QueryTexture(restartInstr, NULL, NULL, &W, &H);
    restartInstrRect = SDL_Rect{graphics->screenWidth()/2 - W/2, gameOverRect.y + gameOverRect.h + H/2, W, H};
}

void HUD::render() {
    // Draw horizontal lines
    SDL_SetRenderDrawColor(graphics->renderer(), 41, 250, 46, 1);
    SDL_RenderDrawLine(graphics->renderer(), 7, graphics->screenHeight() - 40, graphics->screenWidth() - 7, graphics->screenHeight() - 40);
    SDL_RenderDrawLine(graphics->renderer(), 7, graphics->screenHeight() - 41, graphics->screenWidth() - 7, graphics->screenHeight() - 41);
    
    // Render default text
    SDL_RenderCopy(graphics->renderer(), score, NULL, &scoreRect);
    SDL_RenderCopy(graphics->renderer(), lives, NULL, &livesRect);
}

void HUD::renderLives(const int& lifecount) {
    std::string message = std::to_string(lifecount);
    SDL_Texture* livesCount = graphics->renderText(message, "Fonts/space_invaders.ttf", greenColour, 20);
    int W; int H;
    SDL_QueryTexture(livesCount, NULL, NULL, &W, &H);
    SDL_Rect livesCountRect {livesRect.x + livesRect.w + 5, 5, W, H };
    SDL_RenderCopy(graphics->renderer(), livesCount, NULL, &livesCountRect);
}

void HUD::renderScore(const int& playerscore) {
    // Update score number
    SDL_Color greenColour { 41, 250, 46, 1 };
    std::string score = std::to_string(playerscore);
    currentScore = graphics->renderText(score, "Fonts/space_invaders.ttf", greenColour, 20);
    // Update score positioning & render
    int W; int H;
    SDL_QueryTexture(currentScore, NULL, NULL, &W, &H);
    currentScoreRect = SDL_Rect{scoreRect.x + scoreRect.w + 5, scoreRect.y, W, H};
    SDL_RenderCopy(graphics->renderer(), currentScore, NULL, &currentScoreRect);
}

void HUD::renderPause() {
    // Create transparent black/grey overlay
    SDL_Rect screenoverlay {0, 0, graphics->screenWidth(), graphics->screenHeight()};
    SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(graphics->renderer(), 30, 30, 30, 200);
    SDL_RenderFillRect(graphics->renderer(), &screenoverlay);
    
    int W; int H;
    SDL_QueryTexture(pause, NULL, NULL, &W, &H);
    SDL_Rect pauserect { graphics->screenWidth()/2 - W/2, graphics->screenHeight()/2 - H/2, W, H };
    SDL_RenderCopy(graphics->renderer(), pause, NULL, &pauserect);
    
    // Reset render mode
    SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_NONE);
}

void HUD::renderGameOver() {
    // Create transparent black/grey overlay
    SDL_Rect screenoverlay {0, 0, graphics->screenWidth(), graphics->screenHeight()};
    SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(graphics->renderer(), 30, 30, 30, 200);
    SDL_RenderFillRect(graphics->renderer(), &screenoverlay);
    // Render game over text
    SDL_RenderCopy(graphics->renderer(), gameOver, NULL, &gameOverRect);
    SDL_RenderCopy(graphics->renderer(), restartInstr, NULL, &restartInstrRect);
    // Reset render mode
    SDL_SetRenderDrawBlendMode(graphics->renderer(), SDL_BLENDMODE_NONE);
}