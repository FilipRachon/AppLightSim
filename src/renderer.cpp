#include <iostream>
#include <stdexcept>

#include "renderer.h"

Renderer::Renderer(Window &win)
	: window(win)
{
	sdl_renderer = SDL_CreateRenderer(window.sdl_window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdl_renderer) {
		std::cerr << "SDL Error: " << SDL_GetError();
		throw std::runtime_error("Couldn't create renderer!");
	}

	if (SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND) < 0) {
		std::cerr << "SDL Error: " << SDL_GetError();
		throw std::runtime_error("Couldn't set blend mode!");
	}
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(sdl_renderer);
}

void Renderer::clear()
{
	SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
	SDL_RenderClear(sdl_renderer);
}

void Renderer::draw(Drawable &drawable)
{
	drawable.drawCall(*this);
}

void Renderer::draw(Texture &texture, Rect &texRect, Rect &dstRect)
{
	SDL_RenderCopy(sdl_renderer, texture.sdl_texture, &texRect, &dstRect);
}

void Renderer::display()
{
	SDL_RenderPresent(sdl_renderer);
}

void Renderer::draw(Rect &rect, Color color)
{
	uint8_t R = color>>24;
	uint8_t G = color>>16 & 0xFF;
	uint8_t B = color>>8 & 0xFF;
	uint8_t A = color & 0xFF;

	SDL_SetRenderDrawColor(sdl_renderer, R, G, B, A);
	SDL_RenderDrawRect(sdl_renderer, &rect);
}

Texture::Texture(Renderer &renderer, int w, int h)
{
	sdl_texture = SDL_CreateTexture(
		renderer.sdl_renderer, 
		SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_STREAMING, 
		w, h
	);

	SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
}

Texture::Texture(Renderer &renderer, const char *file)
{
	SDL_Surface *surface = SDL_LoadBMP(file);

	if (!surface) {
		std::cerr << "SDL Error: " << SDL_GetError();
		throw std::runtime_error("Couldn't load texture!");
	}

	sdl_texture = SDL_CreateTextureFromSurface(renderer.sdl_renderer, surface);
	SDL_FreeSurface(surface);

	SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
}

Texture::~Texture()
{
	SDL_DestroyTexture(sdl_texture);
}

Rect Texture::getTextureRect()
{
	int w, h;
	SDL_QueryTexture(sdl_texture, NULL, NULL, &w, &h);
	return {0, 0, w, h};
}

void Texture::updatePixels(std::vector<Color> &pixels)
{
	int w;
	SDL_QueryTexture(sdl_texture, NULL, NULL, &w, NULL);
	SDL_UpdateTexture(sdl_texture, NULL, pixels.data(), w * sizeof(w));
}

void Sprite::drawCall(Renderer &renderer)
{
	renderer.draw(texture, texRect, dstRect);
}

Sprite::Sprite(Texture &tex)
	: texture(tex)
{
	texRect = tex.getTextureRect();
	dstRect = tex.getTextureRect();
}

void Sprite::setRect(Rect rect)
{
	dstRect = rect;
}

void Sprite::setTextureTile(int n, int tileCount)
{
	if (n < 0 || n > tileCount) {
		std::cerr << "Incorrect parameters for a tiled texture!\n";
		return;
	}

	Rect rect = texture.getTextureRect();
	texRect = {
		n * rect.w / tileCount,
		rect.y,
		rect.w / tileCount,
		rect.h
	};
}
