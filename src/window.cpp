#include <iostream>
#include <stdexcept>

#include "window.h"

Window::Window(const char *title, uint32_t w, uint32_t h)
	: width(w), height(h)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL Error: " << SDL_GetError();
		throw std::runtime_error("Couldn't initialize SDL2!");
	}

	sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	if (!sdl_window) {
		std::cerr << "SDL Error: " << SDL_GetError();
		throw std::runtime_error("Couldn't create window!");
	}

	exitEvent = false;
}

Window::~Window()
{
	SDL_DestroyWindow(sdl_window);
}

void Window::processEvents()
{
	mouse.leftPressed = false;
	mouse.leftReleased = false;
	mouse.rightPressed = false;
	mouse.rightReleased = false;
	getMousePos(mouse.x, mouse.y);

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			exitEvent = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				mouse.leftPressed = true;
			if (event.button.button == SDL_BUTTON_RIGHT)
				mouse.rightPressed = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				mouse.leftReleased = true;
			if (event.button.button == SDL_BUTTON_RIGHT)
				mouse.rightReleased = true;
			break;
		case SDL_KEYDOWN:
			if (!event.key.repeat)
				keyboard.keysPressed.push_back(event.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			if (event.key.keysym.scancode == SDL_SCANCODE_Q)
				exitEvent = true;
			keyboard.keysReleased.push_back(event.key.keysym.scancode);
			break;
		}
	}  
}

bool Window::wasLeftMousePressed() const
{
	return mouse.leftPressed;
}

bool Window::wasLeftMouseReleased() const
{
	return mouse.leftReleased;
}

bool Window::wasKeyPressed(SDL_Scancode code) const
{
	for (auto key : keyboard.keysPressed) {
		if (key == code)
			return true;
	}
	return false;
}

bool Window::wasKeyReleased(SDL_Scancode code) const
{
	for (auto key : keyboard.keysReleased) {
		if (key == code)
			return true;
	}
	return false;
}

void Window::getMousePos(int &x, int &y) const
{
	SDL_GetMouseState(&x, &y);
}

MouseState Window::getMouseState() const
{
	return mouse;
}

bool Window::shouldExit() const
{
	return exitEvent;
}
