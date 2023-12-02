#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "Renderer.h"

int main(int argc, char* argv[])
{
	Renderer* renderer = new Renderer(800, 600, "MDM", 60);
	while (!renderer->shouldClose) {
		renderer->Loop();
	}
	delete renderer;
	return 0;
}