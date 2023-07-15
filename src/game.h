#ifndef __GAME_H__
#define __GAME_H__
#else
#error "game.h already included somewhere else"
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <assert.h>
#include "core.h"
#include "gameObjects/gameObject.h"
#include "gameObjects/tree.h"
#include "gameObjects/stoneBlock.h"
#include "gameObjects/runningMan.h"
#include "gameObjects/frameCounter.h"
#include "game_t.h"

#define MAX_NODES 100000

Node nodes[MAX_NODES] = {0};
int nodesCount = 0;

Node* getFreeNode()
{
	assert(nodesCount+1 < MAX_NODES && "Max amount of nodes reached");
	return &nodes[nodesCount++];
}

void updateComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node->data;
	if(gameObject != NULL && gameObject->update != NULL)
		gameObject->update(gameObject, game);
}

void renderComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node->data;
	if(gameObject != NULL && gameObject->draw != NULL)
		gameObject->draw(gameObject, game);
}

Game gameCreate()
{
	Game game = {0};
	game.isRunning = true;

	game.screenSize[0] = 800;
	game.screenSize[1] = 600;

	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0 && "Error initializing SDL\n");

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	game.window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		game.screenSize[0],
		game.screenSize[1],
		0);

	assert(game.window != NULL && "Error creating SDL window");

 	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(game.renderer != NULL && "Error creating renderer");

	// game.assetManager = assetManagerInit(game.renderer);

	return game;
}

void gameInit(Game *game)
{
	nodes[nodesCount++].data = gameObjectCreate();
	game->root = &nodes[nodesCount-1];

	{
		Node* node = getFreeNode();
		node->data = runningManCreate((float[]){50, 50}, game->assetManager.textures[ASSET_IMAGE_RUNNING_MAN]);
		game->hero = node->data;
		nodeAddChild(game->root, node);
	}
	{
		Node *node = getFreeNode();
		node->data = frameCounterCreate(game->assetManager.fonts[ASSET_FONT_CHARRIOT]);
		nodeAddChild(game->root, node);
	}
	for(int i = 0; i < game->screenSize[0] / 32; i++)
		for(int j = 0; j < game->screenSize[0] / 24; j++)
		{
			if(((i != 0 && j != 0) && (i != 24 && j != 24) || j > 24)) continue;
			Node* node = getFreeNode();
			node->data = stoneBlockCreate((float[]){i * 32, j * 23 + 25}, game->assetManager.textures[ASSET_IMAGE_STONE_BLOCK]);
			nodeAddChild(game->root, node);
			game->staticColliders[game->staticColliderCount++] = node->data;
		}
}

void swapData(Node* a, Node* b) {
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Comparator function to compare two nodes
typedef int (*Comparator)(const void*, const void*);

// Function to sort the linked list using a comparator function pointer
void sortLinkedList(Node* head, Comparator compare) {
    if (head == NULL || head->next == NULL)
        return;

    int swapped;
    Node* ptr1;
    Node* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != NULL) {
			Node* nn = ptr1->next;
            if (compare(ptr1->data, nn->data) > 0) {
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

int compareGo(GameObject* a, GameObject* b)
{
	return (a->rigidBody.boundingBox.y + a->sprite.center[1] - a->sprite.zIndex) - (b->rigidBody.boundingBox.y + b->sprite.center[1] - b->sprite.zIndex);
}

void gameRender(Game game)
{
	sortLinkedList(game.root->children, compareGo);

	SDL_SetRenderDrawColor(game.renderer, 21, 21, 21, 255);
	SDL_RenderClear(game.renderer);
	traverseGraph(game.root, &game, renderComponentCallback);

	SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
	
	SDL_RenderPresent(game.renderer);
}

void resolveCollision(SDL_FRect *rectA, SDL_FRect *rectB, bool firstIsFixed)
{
	SDL_FRect result;

	if(SDL_IntersectFRect(rectA, rectB, &result))
	{
		if(result.w < result.h)
		{
			if(result.x <= rectB->x)
				rectB->x += result.w;
			else
				rectB->x -= result.w;
		} else 
		{
			if(result.y <= rectB->y)
				rectB->y += result.h;
			else
				rectB->y -= result.h;
		}
	}
}

void resolveCollisionsWithMap(Game* game)
{
	for(int i = 0; i < game->staticColliderCount; i++)
	{
		resolveCollision(&game->staticColliders[i]->rigidBody.boundingBox, &game->hero->parent.rigidBody.boundingBox, true);
	}
}

void gameRun(Game* game)
{
	float lastFrameTime = SDL_GetTicks();
	while(game->isRunning)
	{
		game->deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000. ;
		lastFrameTime = SDL_GetTicks();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// Handle SDL events
			switch (event.type)
			{
				case SDL_QUIT:
					game->isRunning = false;
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						game->isRunning = false;
						break;
					}
					
					game->keys[event.key.keysym.scancode] = true;
					break;
				case SDL_KEYUP:
					game->keys[event.key.keysym.scancode] = false;
					if(event.key.keysym.sym == SDLK_1)
					{
						game->isDebugMode = !game->isDebugMode;
						break;
					}
					break;
			}
		}
		traverseGraph(game->root, game, (TraverseNodeCallback)updateComponentCallback);

		resolveCollisionsWithMap(game);

		gameRender(*game);
	}
}

void gameDestroy(Game game)
{
	traverseGraph(game.root, &game, freeNode);
	assetManagerDestroy(game.assetManager);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
}
