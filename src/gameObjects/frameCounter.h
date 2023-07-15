#include "gameObject.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct
{
	GameObject parent;
	TTF_Font* font;
	SDL_Color textColor;
	Uint32 frameCount;
	Uint32 startTime;
} FrameCounter;

void frameCounterUpdate(FrameCounter* _this, Game* game)
{
	
}

void frameCounterDraw(FrameCounter* _this, Game* game)
{
	Uint32 currentTime = SDL_GetTicks();
	Uint32 elapsedTime = currentTime - _this->startTime;
	_this->frameCount++;
		float fps = (float)_this->frameCount / (float)elapsedTime * 1000.0f;
		char fpsText[16];
		snprintf(fpsText, sizeof(fpsText), "FPS: %.2f", fps);
		SDL_Surface* textSurface = TTF_RenderText_Blended(_this->font, fpsText, _this->textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
		SDL_Rect textRect = {_this->parent.rigidBody.boundingBox.x, _this->parent.rigidBody.boundingBox.y, textSurface->w, textSurface->h}; // position of the FPS text
		SDL_RenderCopy(game->renderer, textTexture, NULL, &textRect);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	if (elapsedTime >= 1000) {
		_this->frameCount = 0;
		_this->startTime = currentTime;
	}
}

FrameCounter* frameCounterCreate(TTF_Font* font)
{
	FrameCounter* frameCounter = calloc(1, sizeof(FrameCounter));
	frameCounter->textColor.r = 255;
	frameCounter->textColor.g = 255;
	frameCounter->textColor.b = 255;
	frameCounter->textColor.a = 255;
	frameCounter->font = font;
	frameCounter->parent.update = frameCounterUpdate;
	frameCounter->parent.draw = frameCounterDraw;

   	frameCounter->frameCount = 0;
    frameCounter->startTime = SDL_GetTicks();

	return frameCounter;
}