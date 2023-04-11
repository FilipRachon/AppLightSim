#include <cmath>

#include "shapes.h"

bool isPointLeftOfLine(int x, int y, int line1X, int line1Y, int line2X, int line2Y) 
{
	int x1 = line2X - line1X;
	int x2 = x - line1X;

	int y1 = line2Y - line1Y;
	int y2 = y - line1Y;

	return x1*y2-x2*y1 <= 0;
}

bool isPointRightOfLine(int x, int y, int line1X, int line1Y, int line2X, int line2Y) 
{
	int x1 = line2X - line1X;
	int x2 = x - line1X;

	int y1 = line2Y - line1Y;
	int y2 = y - line1Y;

	return x1*y2-x2*y1 > 0;
}

TriangleShape::TriangleShape()
{
	x1 = 0; y1 = 0;
	x2 = 0; y2 = 0;
	x3 = 0; y3 = 0;
}

void TriangleShape::setOrigin(int x, int y) 
{
	x1 = x - 30; y1 = y + 10;
	x2 = x + 30; y2 = y + 30;
	x3 = x + 10; y3 = y - 30;
}

bool TriangleShape::isPointInside(int x, int y) 
{
	if (isPointLeftOfLine(x, y, x1, y1, x2, y2) && 
		isPointLeftOfLine(x, y, x2, y2, x3, y3) &&
		isPointLeftOfLine(x, y, x3, y3, x1, y1))
		return true;
	return false;
}

bool TriangleShape::isPointLit(int x, int y, int lightX, int lightY) 
{
	if (isPointInside(x, y))
		return false;

	int leftX = x1, leftY = y1;
	int rightX = x1, rightY = y1;

	if (isPointLeftOfLine(x2, y2, lightX, lightY, leftX, leftY)) {
		leftX = x2; leftY = y2;
	}
	if (isPointLeftOfLine(x3, y3, lightX, lightY, leftX, leftY)) {
		leftX = x3; leftY = y3;
	}

	if (isPointRightOfLine(x2, y2, lightX, lightY, rightX, rightY)) {
		rightX = x2; rightY = y2;
	}
	if (isPointRightOfLine(x3, y3, lightX, lightY, rightX, rightY)) {
		rightX = x3; rightY = y3;
	}

	if (isPointRightOfLine(lightX, lightY, leftX, leftY, rightX, rightY) &&
		isPointLeftOfLine(x, y, leftX, leftY, rightX, rightY) &&
		isPointLeftOfLine(x, y, leftX, leftY, lightX, lightY) &&
		isPointRightOfLine(x, y, rightX, rightY, lightX, lightY))
		return false;

	return true;
}


SquareShape::SquareShape()
{
	x1 = 0;
	y1 = 0;
}

void SquareShape::setOrigin(int x, int y) 
{
	x1 = x - 30; y1 = y - 30;
	x2 = x - 30; y2 = y + 30;
	x3 = x + 30; y3 = y + 30;
	x4 = x + 30; y4 = y - 30;
}

bool SquareShape::isPointInside(int x, int y) 
{
	if (x >= x1 && x <= x3 && 
		y >= y1 && y <= y3)
		return true;

	return false;
}

bool SquareShape::isPointLit(int x, int y, int lightX, int lightY) 
{
	if (isPointInside(x, y))
		return false;

	int leftX = x1, leftY = y1;
	int rightX = x1, rightY = y1;

	if (isPointLeftOfLine(x2, y2, lightX, lightY, leftX, leftY)) {
		leftX = x2; leftY = y2;
	}
	if (isPointLeftOfLine(x3, y3, lightX, lightY, leftX, leftY)) {
		leftX = x3; leftY = y3;
	}
	if (isPointLeftOfLine(x4, y4, lightX, lightY, leftX, leftY)) {
		leftX = x4; leftY = y4;
	}

	if (isPointRightOfLine(x2, y2, lightX, lightY, rightX, rightY)) {
		rightX = x2; rightY = y2;
	}
	if (isPointRightOfLine(x3, y3, lightX, lightY, rightX, rightY)) {
		rightX = x3; rightY = y3;
	}
	if (isPointRightOfLine(x4, y4, lightX, lightY, rightX, rightY)) {
		rightX = x4; rightY = y4;
	}

	if (isPointRightOfLine(lightX, lightY, leftX, leftY, rightX, rightY) &&
		isPointLeftOfLine(x, y, leftX, leftY, rightX, rightY) &&
		isPointLeftOfLine(x, y, leftX, leftY, lightX, lightY) &&
		isPointRightOfLine(x, y, rightX, rightY, lightX, lightY))
		return false;

	return true;
}

CircleShape::CircleShape() 
{
	x1 = 0;
	y1 = 0;
	r = 30;
}

void CircleShape::setOrigin(int x, int y) 
{
	x1 = x;
	y1 = y;
}

bool CircleShape::isPointInside(int x, int y) 
{
	int dx = x - x1;
	int dy = y - y1;

	if (dx * dx + dy * dy <= r * r)
		return true;
		
	return false;
}

bool CircleShape::isPointLit(int x, int y, int lightX, int lightY) 
{
	if (isPointInside(x, y))
		return false;

	int v1x = x - lightX;
	int v1y = y - lightY;

	int v2x = x1 - lightX;
	int v2y = y1 - lightY;

	float v1 = sqrt(v1x*v1x + v1y*v1y);

	float k = (v1x*v2x + v1y*v2y)/v1;
	if (k > 0 && k < v1 && std::abs(v2x*v1y - v2y*v1x)/v1 < r)
		return false;
		
	return true;
}