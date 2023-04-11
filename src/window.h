#pragma once
#include <SDL2/SDL.h>
#include <vector>

struct MouseState {
	int x, y;
	bool leftPressed, leftReleased; 
	bool rightPressed, rightReleased;
};

struct KeyboardState {
	std::vector<SDL_Scancode> keysPressed;
	std::vector<SDL_Scancode> keysReleased;
};

class Window {
	SDL_Window *sdl_window;
	const uint32_t width;
	const uint32_t height;

	MouseState mouse;
	KeyboardState keyboard;
	bool exitEvent;

	friend class Renderer;
public:
	Window(const char *title, uint32_t w, uint32_t h);
	~Window();

	void processEvents();
	bool wasLeftMousePressed() const;
	bool wasLeftMouseReleased() const;
	bool wasKeyPressed(SDL_Scancode code) const;
	bool wasKeyReleased(SDL_Scancode code) const;
	void getMousePos(int &x, int &y) const;
	MouseState getMouseState() const;

	bool shouldExit() const;
};

