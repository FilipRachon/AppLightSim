#pragma once
#include "window.h"

class Renderer;
class Drawable;
class Texture;
class Sprite;

typedef SDL_Rect Rect;
typedef uint32_t Color;

class Renderer {
	Window &window;
	SDL_Renderer *sdl_renderer;

	friend class Texture;
public:
	Renderer(Window &win);
	~Renderer();

	void clear();
	void display();

	void draw(Drawable &drawable);
	void draw(Texture &texture, Rect &texRect, Rect &dstRect);
	void draw(Rect &rect, Color color);
};


class Drawable {
protected:
	virtual void drawCall(Renderer &renderer) = 0;
	friend class Renderer;
};


class Texture {
	SDL_Texture *sdl_texture;

	friend class Renderer;
	friend class Sprite;
public:
	Texture(Renderer &r, int w, int h);
	Texture(Renderer &r, const char *file);
	~Texture();

	Rect getTextureRect();
	void updatePixels(std::vector<Color> &pixels);
};


class Sprite : public Drawable {
	Texture &texture;
	Rect texRect;
	Rect dstRect;

	virtual void drawCall(Renderer &renderer) override;
	friend class Renderer;
public:
	Sprite(Texture &tex);

	void setRect(Rect rect);
	void setTextureTile(int n, int tileCount);
};