#pragma once
#include <SDL\SDL_image.h>
#include <SDL\SDL.h>

struct PixelFormat {
	
};
struct Image {
	SDL_Surface* texture = nullptr;
	size_t w = 0;
	size_t h = 0;

};