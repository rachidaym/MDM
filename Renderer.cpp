#include "Renderer.h"

Renderer::Renderer(int screenWidth, int screenHeight, std::string title, int FPS) {
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
	InitWindow(screenWidth, screenHeight, title.c_str());
	MaximizeWindow();
	SetTargetFPS(FPS);
	rlImGuiSetup(true);

	shouldClose = false;
	mainAxes = new Axes(1.5, 30, 4, 0.5, 100);
	mainAxes->origin = Vector2{ float(GetScreenWidth()) / 2, float(GetScreenHeight()) / 2 };
	ui = new UI();
}

Renderer::~Renderer() {
	delete mainAxes;
	rlImGuiShutdown();
	CloseWindow();
}

void Renderer::Loop() {
	shouldClose = WindowShouldClose();
	HandleInput();
	Draw();
}

void Renderer::HandleInput() {
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		Vector2 delta = GetMouseDelta();
		mainAxes->origin = Vector2Add(mainAxes->origin, delta);
	}
	if (IsKeyPressed(KEY_R) && IsKeyDown(KEY_LEFT_CONTROL)) {
		mainAxes->origin = Vector2{ float(GetScreenWidth()) / 2, float(GetScreenHeight()) / 2 };
	}
	float wheel = GetMouseWheelMove();
	if (wheel != 0 && GetMouseX() > 0.25*GetScreenWidth()) {
		mainAxes->_scale += zoomIncrement * wheel;
		if (mainAxes->_scale < zoomIncrement) mainAxes->_scale = zoomIncrement;
	}

	if (selectedObject) {
		if (IsKeyPressed(KEY_UP)) {
			selectedObject->pos.y += moveIncrement;
		}
		if (IsKeyPressed(KEY_DOWN)) {
			selectedObject->pos.y -= moveIncrement;
		}
		if (IsKeyPressed(KEY_RIGHT)) {
			selectedObject->pos.x += moveIncrement;
		}
		if (IsKeyPressed(KEY_LEFT)) {
			selectedObject->pos.x -= moveIncrement;
		}
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT)) {
			inputTimer += GetFrameTime();
		}
		if (inputTimer > holdTime) {
			if (IsKeyDown(KEY_UP)) {
				selectedObject->pos.y += moveIncrement;
			}
			if (IsKeyDown(KEY_RIGHT)) {
				selectedObject->pos.x += moveIncrement;
			}
			if (IsKeyDown(KEY_DOWN)) {
				selectedObject->pos.y -= moveIncrement;
			}
			if (IsKeyDown(KEY_LEFT)) {
				selectedObject->pos.x -= moveIncrement;
			}
		}
		if (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_DOWN) || IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_LEFT)) {
			inputTimer = 0;
		}
	}
}

void Renderer::Draw() {
	BeginDrawing();
	rlImGuiBegin();

	ClearBackground(BACKGROUND_COLOR);


	
	//Axes
	{
		//Horizontal Axis
		DrawLineEx(Vector2{ 0, mainAxes->origin.y }, Vector2{ float(GetScreenWidth()), mainAxes->origin.y}, mainAxes->_thickness, WHITE);
		for (float i = mainAxes->origin.x; i >= 0; i -= mainAxes->_ticksInterval) {
			DrawLineEx(Vector2{ i, mainAxes->origin.y - mainAxes->_ticksLength }, Vector2{ i, mainAxes->origin.y + mainAxes->_ticksLength }, mainAxes->_ticksThickness, LIGHTGRAY);
		}
		for (float i = mainAxes->origin.x; i <= GetScreenWidth(); i += mainAxes->_ticksInterval) {
			DrawLineEx(Vector2{ i, mainAxes->origin.y - mainAxes->_ticksLength }, Vector2{ i, mainAxes->origin.y + mainAxes->_ticksLength }, mainAxes->_ticksThickness, LIGHTGRAY);
		}
		//Vertical Axis
		DrawLineEx(Vector2{ mainAxes->origin.x, 0 }, Vector2{ mainAxes->origin.x, float(GetScreenHeight())}, mainAxes->_thickness, WHITE);
		for (float i = mainAxes->origin.y; i >= 0; i -= mainAxes->_ticksInterval) {
			DrawLineEx(Vector2{ mainAxes->origin.x - mainAxes->_ticksLength,i }, Vector2{ mainAxes->origin.x + mainAxes->_ticksLength ,i }, mainAxes->_ticksThickness, LIGHTGRAY);
		}
		for (float i = mainAxes->origin.y; i <= GetScreenHeight(); i += mainAxes->_ticksInterval) {
			DrawLineEx(Vector2{ mainAxes->origin.x - mainAxes->_ticksLength,i}, Vector2{mainAxes->origin.x + mainAxes->_ticksLength ,i}, mainAxes->_ticksThickness, LIGHTGRAY);
		}
	}

	for (auto i : objects) {
		if (i->mass > 0) {
			if (i == selectedObject) {
				i->Draw(*mainAxes, SELECTEDOBJECT_COLOR);
			}
			else {
				i->Draw(*mainAxes, UNSELECTEDOBJECT_COLOR);
			}
		}
		i->CalculateArea();
		i->CalculateCenter();
	}
	for (auto i : objects) {
		if (i->mass < 0) {
			i->Draw(*mainAxes, BACKGROUND_COLOR);
		}
	}
	if (selectedObject) {
		DrawSelectedCenter(selectedObject);
	}

	center = DrawCenter();

	ui->Show(objects, selectedObject, center);

	rlImGuiEnd();
	EndDrawing();
}

void Renderer::DrawSelectedCenter(Object* object) {
	DrawCircle(mainAxes->_scale*object->center.x + mainAxes->origin.x, mainAxes->origin.y - mainAxes->_scale * object->center.y, mainAxes->_scale*0.05, RED);
}

Vector2 Renderer::DrawCenter() {
	float topx = 0;
	float topy = 0;
	float bottom = 0;
	for (auto o : objects) {
		topx += o->center.x * o->area;
		topy += o->center.y * o->area;
		bottom += o->area;
	}
	center.x = topx / bottom;
	center.y = topy / bottom;
	DrawCircle(mainAxes->_scale * center.x + mainAxes->origin.x, mainAxes->origin.y - mainAxes->_scale * center.y, mainAxes->_scale * 0.05, GOLD);
	return center;
}