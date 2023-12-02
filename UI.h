#pragma once
#include "imgui.h"
#include "raylib.h"
#include "Objects.h"
#include <vector>
#include <string>
#include <cstdlib>

class Window {
public:
	bool open;
	Window() { open = true; }
	virtual void Show(std::vector<Object*>& objects, Object*& selected, Vector2 center, float area, std::vector<Window*>& windows) {};
};

class ObjectCreator : public Window {
public:
	int selection = 0;
	char name[20] = "Untitled";
	virtual void Show(std::vector<Object*>& objects, Object*& selected, Vector2 center, float area, std::vector<Window*>& windows);
};

class MomentCalculator : public Window {
public:
	bool resultsOpen = true;
	float axisAngle = 0;

	float Ix = 0;
	float Iy = 0;
	float Ixy = 0;

	float Ixc = 0;
	float Iyc = 0;
	float Ixyc = 0;
	
	float Ixa = 0;
	float Iya = 0;
	float Ixya = 0;
	
	float Ip1 = 0;
	float Ip2 = 0;
	float principalAngle = 0;
	virtual void Show(std::vector<Object*>& objects, Object*& selected, Vector2 center, float area, std::vector<Window*>& windows);
	void Calculate(std::vector<Object*>& objects, Vector2 center, float area);
};

class ShowResults : public Window {
public:
	MomentCalculator moment;
	ShowResults(MomentCalculator);
	virtual void Show(std::vector<Object*>& objects, Object*& selected, Vector2 center, float area,std::vector<Window*>& windows);
};

class UI
{
public:
	std::vector<Window*> windows;
	bool open;
	float totalArea = 0;
	void Show(std::vector<Object*>& objects, Object*& selected, Vector2 center);
	UI() { open = true; };
};

