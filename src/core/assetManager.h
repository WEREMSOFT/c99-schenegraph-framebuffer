#ifndef __ASSET_MANAGER_H__
#define __ASSET_MANAGER_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

typedef enum 
{	
	ASSET_IMAGE_TREE,
	ASSET_IMAGE_CHOPTER_SPRITESHEET,
	ASSET_IMAGE_RUNNING_MAN,
	ASSET_IMAGE_STONE_BLOCK,
	ASSET_IMAGE_PLACEHOLDER_CUBE,
	ASSET_IMAGE_COUNT
} AssetImageEnum;

typedef enum {
	ASSET_FONT_CHARRIOT,
	ASSET_FONT_COUNT
} AssetFontsEnum;

typedef struct {
	SDL_Texture* textures[ASSET_IMAGE_COUNT];
	TTF_Font* fonts[ASSET_FONT_COUNT];
} AssetManager;

static void loadTexture(AssetManager *assetManager, SDL_Renderer* renderer, AssetImageEnum textureId, const char* imagePath)
{
	SDL_Surface* surface = IMG_Load(imagePath);
	assert(surface != NULL && "error loading image");

	assetManager->textures[textureId]  = SDL_CreateTextureFromSurface(renderer, surface);
	assert(assetManager->textures[textureId] != NULL && "error loading texture");

	SDL_FreeSurface(surface);
	
}

static void loadFont(AssetManager* assetManager, AssetFontsEnum fontId, const char* fontPath, int fontSize)
{
	assetManager->fonts[fontId] = TTF_OpenFont(fontPath, fontSize);
	assert(assetManager->fonts[fontId] != NULL && "Error loading font");
}

AssetManager assetManagerInit(SDL_Renderer* renderer)
{
	assert(TTF_Init() >= 0 && "Error initilizing font library");
	AssetManager assetManager = {0};
	
	loadTexture(&assetManager,  renderer, ASSET_IMAGE_TREE, "assets/tree.png");
	loadTexture(&assetManager,  renderer, ASSET_IMAGE_CHOPTER_SPRITESHEET, "assets/chopper-spritesheet.png");
	loadTexture(&assetManager,  renderer, ASSET_IMAGE_RUNNING_MAN, "assets/running-man.png");
	loadTexture(&assetManager,  renderer, ASSET_IMAGE_STONE_BLOCK, "assets/stone-block.png");
	loadTexture(&assetManager,  renderer, ASSET_IMAGE_PLACEHOLDER_CUBE, "assets/placeholder-cube.png");
	loadFont(&assetManager, ASSET_FONT_CHARRIOT, "assets/fonts/charriot.ttf", 12);

	return assetManager;
}

void assetManagerDestroy(AssetManager assetManager)
{
	for(int i = 0; i < ASSET_IMAGE_COUNT; i++)
	{
		SDL_DestroyTexture(assetManager.textures[i]);
	}

	for(int i = 0; i < ASSET_FONT_COUNT; i++)
	{
		TTF_CloseFont(assetManager.fonts[i]);
	}
    TTF_Quit();
}

#endif