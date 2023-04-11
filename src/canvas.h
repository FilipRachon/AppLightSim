#pragma once
#include <vector>

#include "renderer.h"
#include "shapes.h"

#define CANVAS_WIDTH 380
#define CANVAS_HEIGHT 380
#define CANVAS_X 20
#define CANVAS_Y 20

class Canvas : public Drawable {
	Renderer &renderer;
	Window &window;

	std::vector<Color> pixels;
	Texture background;
	Texture foreground;
	Sprite back;
	Sprite front;
	
	std::vector<Shape*> shapes;
	bool moving;
	Shape *movingShape;
	Shape *highlighedShape;

	int lightX;
	int lightY;
	float lightRadius;
	Color lightColor;

	virtual void drawCall(Renderer &renderer) override;
	friend class Renderer;
public:
	Canvas(Renderer &rend, Window &win);

	void update();

	void addTriangle();
	void addSquare();
	void addCircle();
	void removeShapes();
	void setLightColor(Color color);
	void setLightRadius(float r);
	bool isMouseInside();
};