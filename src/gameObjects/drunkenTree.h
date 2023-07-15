#include <SDL2/SDL.h>
#include <math.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"
typedef struct 
{
	GameObject parent;
	float phase;
	float radialSpeed;
	float radious;
} DrunkenTree;

void drunkenTreeDraw(DrunkenTree* _this, Game* game)
{
	int x = (int)_this->parent.rigidBody.position[0];
	int y = (int)_this->parent.rigidBody.position[1];
	SDL_RenderCopyEx(game->renderer, _this->parent.sprite.texture, &((SDL_Rect){0, 0, 16, 32}), &((SDL_Rect){x, y, 16, 32}), 0, NULL, SDL_FLIP_NONE);
}

void drunkenTreeUpdate(DrunkenTree* _this, Game* game)
{
	_this->phase += game->deltaTime * _this->radialSpeed;
	_this->parent.rigidBody.position[0] = sinf(_this->phase) * _this->radious + _this->parent.rigidBody.initialPosition[0];
	_this->parent.rigidBody.position[1] = cosf(_this->phase) * _this->radious + _this->parent.rigidBody.initialPosition[1];
}

DrunkenTree* drunkenTreeCreate(float position[2], float speed, SDL_Texture* texture)
{
	DrunkenTree* drunkenTree = (DrunkenTree*)malloc(sizeof(DrunkenTree));
	drunkenTree->parent.rigidBody.initialPosition[0] = position[0];
	drunkenTree->parent.rigidBody.initialPosition[1] = position[1];
	
	drunkenTree->phase = sinf(position[0]/100.) + cosf(position[1]/100.);
	drunkenTree->radious = 20.;
	drunkenTree->radialSpeed = 10.;

	drunkenTree->parent.update = (UpdateFunction)drunkenTreeUpdate;
	drunkenTree->parent.draw = (DrawFunction)drunkenTreeDraw;
	drunkenTree->parent.rigidBody.speed = speed;
	drunkenTree->parent.sprite.center[1] = 40;
	drunkenTree->parent.sprite.texture = texture;
	return drunkenTree;
}
