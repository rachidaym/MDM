#pragma once
#include "raylib.h"
class Axes
{
public:
	float _thickness;
	float _ticksInterval;
	float _ticksLength;
	float _ticksThickness;
	float _scale;
	Vector2 origin;
	Axes(float thickness, float ticksInterval, float ticksLength, float ticksThickness, float scale);
};

