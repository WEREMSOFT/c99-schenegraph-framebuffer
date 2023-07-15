#include <SDL2/SDL.h>
#include <math.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"
typedef struct 
{
	GameObject parent;
} StoneBlock;

void stoneBlockDraw(StoneBlock* _this, Game* game)
{
	gameObjectDraw(&_this->parent, game);
}

void stoneBlockUpdate(StoneBlock* _this, Game* game)
{
}

StoneBlock* stoneBlockCreate(float position[2], SDL_Texture* texture)
{
	StoneBlock* stoneBlock = (StoneBlock*)malloc(sizeof(StoneBlock));
	stoneBlock->parent.rigidBody.boundingBox.x = position[0];
	stoneBlock->parent.rigidBody.boundingBox.y = position[1];
	
	stoneBlock->parent.rigidBody.boundingBox.w = 32;
	stoneBlock->parent.rigidBody.boundingBox.h = 23;

	int size[2];

	SDL_QueryTexture(texture, NULL, NULL, &size[0], &size[1]);

	stoneBlock->parent.sprite.srcRect = (SDL_Rect){0, 0, size[0], size[1]};
	stoneBlock->parent.sprite.destRect = (SDL_Rect){0, 0, size[0], size[1]};

	stoneBlock->parent.update = (UpdateFunction)stoneBlockUpdate;
	stoneBlock->parent.draw = (DrawFunction)stoneBlockDraw;

	stoneBlock->parent.sprite.offset[0] = 0;
	stoneBlock->parent.sprite.offset[1] = 21;
	stoneBlock->parent.sprite.center[1] = 0;
	stoneBlock->parent.sprite.zIndex = 27;

	stoneBlock->parent.sprite.isFlipped = false;
	stoneBlock->parent.sprite.isAnimated = false;

	stoneBlock->parent.sprite.texture = texture;
	return stoneBlock;
}
