#include <Gamma/Gamma.h>
#include <Gamma/Window/Window.h>
#include <Gamma/Renderer/Renderer.h>
#include <iostream>

using namespace Gamma;

int main() {
	GammaInit();
	WindowClass WndClass;
	Window Window;
	Window.OpenWindow(WndClass, "Gamma", 1280, 720);
	while (Window.GetState()) {
		Window.PollEvents();
	}
	Window.CloseWindow();
	GammaQuit();
	return 0;
}