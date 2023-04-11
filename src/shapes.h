#pragma once
#include <vector>
#include <cmath>

class Shape {
public:
	virtual ~Shape() = default;
	virtual void setOrigin(int x, int y) = 0;
	virtual bool isPointInside(int x, int y) = 0;
	virtual bool isPointLit(int x, int y, int lightX, int lightY) = 0;
};

class TriangleShape : public Shape {
	int x1, y1;
	int x2, y2;
	int x3, y3;
public:
	TriangleShape();

	virtual void setOrigin(int x, int y) override;
	virtual bool isPointInside(int x, int y) override;
	virtual bool isPointLit(int x, int y, int lightX, int lightY) override;
};


class SquareShape : public Shape {
	int x1, y1;
	int x2, y2;
	int x3, y3;
	int x4, y4;
public:
	SquareShape();

	virtual void setOrigin(int x, int y) override;
	virtual bool isPointInside(int x, int y) override;
	virtual bool isPointLit(int x, int y, int lightX, int lightY) override;
};

class CircleShape : public Shape {
	int x1, y1, r;
public:
	CircleShape();

	virtual void setOrigin(int x, int y) override;
	virtual bool isPointInside(int x, int y) override;
	virtual bool isPointLit(int x, int y, int lightX, int lightY) override;
};