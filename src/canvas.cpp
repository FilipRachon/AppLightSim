#include <cmath>

#include "canvas.h"

#define CANVAS_WIDTH 380
#define CANVAS_HEIGHT 380
#define CANVAS_X 20
#define CANVAS_Y 20

Canvas::Canvas(Renderer &rend, Window &win) 
	: renderer(rend), window(win), pixels(CANVAS_WIDTH * CANVAS_HEIGHT, 0x000000FF),
	background(rend, CANVAS_WIDTH, CANVAS_HEIGHT), foreground(rend, CANVAS_WIDTH, CANVAS_HEIGHT), 
	back(background), front(foreground) 
{

	background.updatePixels(pixels);
	lightColor = 0xFFFF00FF;
	lightX = 200;
	lightY = 200;
	lightRadius = 200.0;

	back.setRect({CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT});
	front.setRect({CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT});

	moving = false;
	movingShape = nullptr;
	highlighedShape = nullptr;
}

void Canvas::update() 
{
	int xm, ym;
	window.getMousePos(xm, ym);
	xm -= CANVAS_X;
	ym -= CANVAS_Y;

	bool mouseInside = isMouseInside();

	if (movingShape) {
		movingShape->setOrigin(xm, ym);
		if (window.getMouseState().leftReleased && mouseInside) {
			moving = false;
			movingShape = nullptr;
		}
	}

	for (auto shape : shapes) {
		if (shape->isPointInside(lightX, lightY)) {
			std::fill(pixels.begin(), pixels.end(), 0x00000000);
			foreground.updatePixels(pixels);
			return;
		}
	}

	if (moving) {
		highlighedShape = movingShape;
	} else {
		highlighedShape = nullptr;
		for (auto shape : shapes) {
			if (shape->isPointInside(xm, ym)) {
				highlighedShape = shape;
				break;
			}
		}
	}

	if (!moving && highlighedShape != nullptr && window.getMouseState().leftPressed) {
		moving = true;
		movingShape = highlighedShape;
	}

	for (int y = 0; y < CANVAS_HEIGHT; y++) {
		for (int x = 0; x < CANVAS_WIDTH; x++) {
			int r = lightRadius;
			int dx = lightX - x;
			int dy = lightY - y;

			if (dx*dx + dy*dy > r*r) {
				pixels[y*CANVAS_WIDTH+x] = 0x00000000;
				continue;
			}

			float d = sqrt(dx*dx+dy*dy);
			if (d > lightRadius) {
				pixels[y*CANVAS_WIDTH+x] = 0x00000000;
				continue;
			}

			bool skip = false;
			if (mouseInside || moving) {
				for (auto shape : shapes) {
					if (shape->isPointInside(x, y)) {
						if (shape == highlighedShape)
							pixels[y*CANVAS_WIDTH+x] = 0xFF00FFFF;
						else 
							pixels[y*CANVAS_WIDTH+x] = 0xA0B0C0FF;
						skip = true;
						break;
					}
				}
			}

			if (skip)
				continue;

			for (auto shape : shapes) {
				if (!shape->isPointLit(x, y, lightX, lightY)) {
					pixels[y*CANVAS_WIDTH+x] = 0x00000000;
					skip = true;
					break;
				}
			}

			if (skip)
				continue;

			uint8_t R = lightColor>>24;
			uint8_t G = lightColor>>16;
			uint8_t B = lightColor>>8;
			uint8_t A = 255.0 * (1.0 - d / lightRadius);
			pixels[y*CANVAS_WIDTH+x] = (R<<24) | (G<<16) | (B<<8) | A;
		}
	}
	foreground.updatePixels(pixels);
}

void Canvas::addTriangle() 
{
	movingShape = new TriangleShape;
	shapes.push_back(movingShape);
	moving = true;
}

void Canvas::addSquare() 
{
	movingShape = new SquareShape;
	shapes.push_back(movingShape);
	moving = true;
}

void Canvas::addCircle() 
{
	movingShape = new CircleShape;
	shapes.push_back(movingShape);
	moving = true;
}

void Canvas::removeShapes() 
{
	highlighedShape = movingShape = nullptr;
	moving = false;
	for (auto shape : shapes) {
		delete shape;
	}
	shapes.clear();
}

void Canvas::setLightColor(Color color) 
{
	lightColor = color;
}

void Canvas::setLightRadius(float r) 
{
	lightRadius = r;
}

bool Canvas::isMouseInside() 
{
	int x, y;
	window.getMousePos(x, y);
	return (x > CANVAS_X && x < CANVAS_X + CANVAS_WIDTH && 
		y > CANVAS_Y && y < CANVAS_Y + CANVAS_HEIGHT);
}

void Canvas::drawCall(Renderer &renderer) 
{
	renderer.draw(back);
	renderer.draw(front);
}