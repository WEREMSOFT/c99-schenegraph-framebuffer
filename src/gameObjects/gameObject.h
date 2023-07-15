#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__
#include <SDL2/SDL.h>
#include "../core.h"
#include "../game_t.h"
#include <stddef.h>

typedef void(*UpdateFunction)(const void*, Game*);
typedef void(*DrawFunction)(const void*, Game*);

typedef struct
{
	int framesPerSecond;
	int frameCount;
	int currentFrame;
	int startTime;
	int currentAnimation;
} Animation;

typedef struct 
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Texture* texture;
	Animation animation;
	int center[2];
	int offset[2];
	int zIndex;
	bool isAnimated;
	bool isFlipped;
} Sprite;

typedef struct 
{
	float initialPosition[2];
	SDL_FRect boundingBox;
	float speed;
} RigidBody;

typedef struct GameObject
{
	Sprite sprite;
	RigidBody rigidBody;
	UpdateFunction update;
	DrawFunction draw;
} GameObject;

GameObject* gameObjectCreate()
{
	GameObject go = {0};
	GameObject* gameObject = malloc(sizeof(GameObject));
	*gameObject = go;
	return gameObject;
}

Animation animationCreate(int frameCount, int framesPerSecond, bool isLoop)
{
	Animation animation = {0};
	animation.frameCount = frameCount;
	animation.framesPerSecond = framesPerSecond;
	return animation;
}

SDL_Rect processAnimationFrame(GameObject* _this)
{
	Sprite* sprite = &_this->sprite;
	Animation* animation = &sprite->animation;
	animation->currentFrame = ((SDL_GetTicks() - animation->startTime) * animation->framesPerSecond / 1000) % animation->frameCount;
	sprite->srcRect.x = animation->currentFrame * sprite->srcRect.w;
	sprite->srcRect.y = animation->currentAnimation * sprite->srcRect.h;
}

void gameObjectDraw(GameObject* _this, Game* game)
{
	_this->sprite.destRect.x = (int)_this->rigidBody.boundingBox.x - _this->sprite.offset[0];
	_this->sprite.destRect.y = (int)_this->rigidBody.boundingBox.y - _this->sprite.offset[1];
	SDL_RenderCopyEx(game->renderer, _this->sprite.texture, &_this->sprite.srcRect, &_this->sprite.destRect, 0, NULL, _this->sprite.isFlipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);

	if(game->isDebugMode)
	{
		SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
		SDL_RenderDrawRectF(game->renderer, &_this->rigidBody.boundingBox);
	}
}

#endif