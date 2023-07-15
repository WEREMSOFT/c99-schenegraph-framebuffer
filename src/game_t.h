#ifndef __GAME_T_H__
#define __GAME_T_H__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <assert.h>
#include "core.h"
#include "core/assetManager.h"

typedef struct GameObject GameObject;
typedef struct RunningMan RunningMan;

typedef struct Game
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	Node* root;
	AssetManager assetManager;
	bool keys[256];
	bool isRunning;
	bool isDebugMode;
	float deltaTime;
	int screenSize[2];
	int staticColliderCount;
	GameObject* staticColliders[150];
	RunningMan* hero;
} Game;
#endif