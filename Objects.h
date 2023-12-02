#pragma once
#include "raylib.h"
#include "Axes.h"
#include "Constants.h"
#include <iostream>

enum ObjectType{RECT, TRIANGLE, CIRCLE, TRAPEZOID};

class Object
{
public:
	ObjectType type;
	std::string name;
	float mass = 1;
	float area;
	
	float Ix;
	float Iy;
	float Ixy;

	Vector2 center;
	Vector2 pos;
	virtual void Draw(const Axes&, Color) { };
	virtual void CalculateCenter() { };
	virtual void CalculateArea() { };
	virtual void CalculateMOI() { };
};

class Rect : public Object {
public:
	float width;
	float height;
	Rect(char* n, Vector2 position, float w, float h);
	virtual void Draw(const Axes&, Color);
	virtual void CalculateCenter();
	virtual void CalculateArea();
	virtual void CalculateMOI();
};

class Triangle : public Object {
public:
	float height;
	float b1;
	float b2;
	Triangle(char* n, Vector2 position, float h, float totalBase, float diffBase);
	virtual void Draw(const Axes&, Color);
	virtual void CalculateCenter();
	virtual void CalculateArea();
	virtual void CalculateMOI();
};

class Circle : public Object {
public:
	float innerRadius;
	float outerRadius;
	float firstAngle;
	float secondAngle;
	Circle(char* n,Vector2 position, float inner, float outer, float ang1, float ang2);
	virtual void Draw(const Axes&, Color);
	virtual void CalculateCenter();
	virtual void CalculateArea();
	virtual void CalculateMOI();
};

class Trapezoid : public Object {
public:
	float h;
	float bottomBase;
	float topBase;
	Trapezoid(char* n,Vector2 position, float height, float bB, float tB);
	virtual void Draw(const Axes&, Color);
	virtual void CalculateCenter();
	virtual void CalculateArea();
	virtual void CalculateMOI();
};