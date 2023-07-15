#include <SDL2/SDL.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"

typedef GameObject Chopter;

void chopterDraw(Chopter* _this, Game* game)
{
	_this->sprite.destRect.x = (int)_this->rigidBody.position[0];
	_this->sprite.destRect.y = (int)_this->rigidBody.position[1];
	SDL_RenderCopyEx(game->renderer, _this->sprite.texture, &_this->sprite.srcRect, &_this->sprite.destRect, 0, NULL, SDL_FLIP_NONE);
}

void chopterUpdate(Chopter* _this, Game* game)
{
	processAnimationFrame(_this);
	if(game->keys[SDL_SCANCODE_LEFT])
	{
		_this->rigidBody.position[0] -= _this->rigidBody.speed * game->deltaTime;
		_this->sprite.srcRect.y = _this->sprite.srcRect.h * 3;
	}
	if(game->keys[SDL_SCANCODE_RIGHT])
	{
		_this->rigidBody.position[0] += _this->rigidBody.speed * game->deltaTime;
		_this->sprite.srcRect.y = _this->sprite.srcRect.h;
	}
	if(game->keys[SDL_SCANCODE_UP])
	{
		_this->rigidBody.position[1] -= _this->rigidBody.speed * game->deltaTime;
		_this->sprite.srcRect.y = 0;
	}
	if(game->keys[SDL_SCANCODE_DOWN])
	{
		_this->rigidBody.position[1] += _this->rigidBody.speed * game->deltaTime;
		_this->sprite.srcRect.y = _this->sprite.srcRect.h * 2;
	}
}

Chopter* chopterCreate(float position[2], float speed, SDL_Texture* texture)
{
	Chopter* chopter = (Chopter*)malloc(sizeof(Chopter));
	chopter->rigidBody.position[0] = position[0];
	chopter->rigidBody.position[1] = position[1];
	chopter->update = (UpdateFunction)chopterUpdate;
	chopter->draw = (DrawFunction)chopterDraw;
	chopter->rigidBody.speed = speed;
	chopter->sprite.texture = texture;
	chopter->sprite.srcRect = (SDL_Rect){0, 0, 32, 32};
	chopter->sprite.destRect = (SDL_Rect){0, 0, 32, 32};
	chopter->sprite.animation.startTime = SDL_GetTicks();
	chopter->sprite.animation.frameCount = 2;
	chopter->sprite.animation.framesPerSecond = 10;
	return chopter;
}
