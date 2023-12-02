#pragma once
#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "imgui_impl_raylib.h"
#include "rlImGui.h"
#include "Axes.h"
#include "Objects.h"
#include "UI.h"
#include <iostream>
#include <vector>
class Renderer
{
private:
	Axes* mainAxes;
	UI* ui;
public:
	bool shouldClose;
	std::vector<Object*> objects;
	Object* selectedObject = nullptr;
	const float zoomIncrement = 2;
	Vector2 center;
	float inputTimer=0;
	float moveIncrement = 0.1;
	float holdTime = 0.25;

	Renderer(int screenWidth, int screenHeight, std::string title, int FPS);
	~Renderer();
	void Loop();
	void HandleInput();
	void Draw();
	void DrawUI();
	void DrawSelectedCenter(Object*);
	Vector2 DrawCenter();
};

