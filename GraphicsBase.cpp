// GraphicsBase.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GraphicsBase.h"








Skeleton::Skeleton(int width, int height) {
	screenWidth = width;
	screenHeight = height;
	init();
}
Skeleton::~Skeleton() {
	close();
}

bool Skeleton::init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow("Skeleton (SDL)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

int Skeleton::loadMedia(char* filepath) {
	SDL_Surface* media = NULL;

	media = SDL_LoadBMP(filepath);
	
	if (media == NULL) {
		printf("Unable to load image %s! SDL ERROR:%s\n", "filepath", SDL_GetError());
	}
	else {
		return addSurface(media);
	}
	return -1;
}

int Skeleton::createTexture(Uint32* pixels, int w, int h) {
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);
	SDL_UpdateTexture(texture, NULL, pixels, w * sizeof(Uint32));
	Skin* skin = new Skin;
	skin->texture = texture;
	skin->w = w;
	skin->h = h;
	return addSkin(skin);
}

int Skeleton::createSprite(int textureindex, int x, int y)
{
	Sprite* sprite = new Sprite;
	sprite->skin = skins[textureindex];
	sprite->x = x;
	sprite->y = y;
	return addSprite(sprite);
}

int Skeleton::addSurface(SDL_Surface* surface) {
	if (surface == NULL)
		return -1;
	numSurfaces++;
	if (numSurfaces >= curSurfaceSize) {
		surfaces = (SDL_Surface**)realloc(surfaces, sizeof(SDL_Surface*) * numSurfaces * 2);
		curSurfaceSize = numSurfaces * 2;
	}
	surfaces[numSurfaces - 1] = surface;
	return numSurfaces - 1;
}
int Skeleton::addSkin(Skin* skin) {
	if (skin == NULL)
		return -1;
	numSkins++;
	if (numSkins >= curSkinSize) {
		skins = (Skin**)realloc(skins, sizeof(Skin*) * numSkins * 2);
		curSkinSize = numSkins * 2;
	}
	skins[numSkins - 1] = skin;
	return numSkins - 1;
}

int Skeleton::addSprite(Sprite* sprite) {
	if (sprite == NULL)
		return -1;
	numSprites++;
	if (numSprites >= curSpriteSize) {
		sprites = (Sprite**)realloc(sprites, sizeof(Sprite*) * numSprites * 2);
		curSpriteSize = numSprites * 2;
	}
	sprites[numSprites - 1] = sprite;
	return numSprites - 1;
}

void Skeleton::close() {
	for (int i = 0; i < numSurfaces; i++) {
		SDL_FreeSurface(surfaces[i]);
	}
	for (int i = 0; i < numSkins; i++) {
		SDL_DestroyTexture(skins[i]->texture);
	}
	surfaces = NULL;
	skins = NULL;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}

bool Skeleton::listenExit() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return true;
		}
	}
	return false;
}

void Skeleton::render() {
	SDL_RenderClear(renderer);
	for (int i = 0; i < numSprites; i++) {
		SDL_Rect dstrect;
		dstrect.x = sprites[i]->x;
		dstrect.y = sprites[i]->y;
		dstrect.w = sprites[i]->skin->w;
		dstrect.h = sprites[i]->skin->h;
		SDL_RenderCopy(renderer, sprites[i]->skin->texture, NULL, &dstrect);
	}
	SDL_RenderPresent(renderer);
	//SDL_UpdateWindowSurface(gWindow);
}

