#include <Gamma/Gamma.h>
#include <Gamma/Window/Window.h>
#include <Gamma/Renderer/Renderer.h>
#include <iostream>

using namespace Gamma;

int main() {
	GammaInit();
	WindowClass WndClass;
	WndClass.FullscreenState = FullscreenState::FULLSCREEN;
	Window Window;
	Window.OpenWindow(WndClass, "Gamma", 1280, 720);
	while (!Window.ShouldClose()) {
		Window.PollEvents();
	}
	Window.CloseWindow();
	GammaQuit();
	return 0;
}