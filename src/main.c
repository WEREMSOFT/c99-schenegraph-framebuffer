#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
// #include "game.h"
// #include "core/debugMemory.h"

#define UR_SCREEN_WIDTH 320
#define UR_SCREEN_HEIGHT 200

SDL_Renderer *renderer;

void putPixel(int x, int y, char r, char g, char b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderDrawPoint(renderer, x, y);
}
#define UR_PUT_PIXEL putPixel

int getKeyState(int key)
{
	return 0;
}

#define UR_GET_KEY_STATE getKeyState

#define UR_KEY_PRESS SDL_KEYDOWN

int getButtonState(int mouseButton)
{
	return 0;
}

#define UR_GET_BUTTON_STATE

#include "universal_renderer.h"

int main(void)
{
	// INIT
	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0 && "Error initializing SDL\n");

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	SDL_Window* window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		0);

	assert(window != NULL && "Error creating SDL window");

 	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(renderer != NULL && "Error creating renderer");
	
	SDL_RenderSetScale(renderer, 2., 2.);

	// RUN
	bool isRunning = true;

	Uint32 oldTime = SDL_GetTicks();
	Uint32 newTime;
	Uint32 delta;

	while(isRunning)
	{
		newTime = SDL_GetTicks();
		delta = newTime - oldTime;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    	SDL_RenderClear(renderer);

		for (int i = 0; i < 800; ++i)
			putPixel(i, i, 0, 255, 0);

		if(keys[SDL_SCANCODE_SPACE])
			urDrawCircleFill((URPointI){100, 100}, 100, (URColor){255, 0, 0});
		else
			urDrawCircleFill((URPointI){100, 100}, 100, (URColor){0, 255, 0});

		urPrintString((URPointI){100, 100}, "hello world!!", (URColor){255, 255, 0});

		urPrintFPS((double)(delta / 1000.));

		SDL_RenderPresent(renderer);
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// Handle SDL events
			switch (event.type)
			{
				case SDL_QUIT:
					isRunning = false;
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						isRunning = false;
						break;
					}
					
					keys[event.key.keysym.scancode] = true;
					break;
				case SDL_KEYUP:
					keys[event.key.keysym.scancode] = false;
					break;
			}
		}
		oldTime = newTime;
	}

	// DESTROY
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	// Game game = gameCreate();
	// gameInit(&game);
	// gameRun(&game);
	// gameDestroy(game);
	return 0;
}