#include <Gamma/Gamma.h>
#include <Gamma/Window/Window.h>
#include <Gamma/Renderer/Renderer.h>
#include <iostream>

using namespace Gamma;

int main() {
	GammaInit();
	WindowClass WndClass;
	WndClass.FullscreenState = FullscreenState::WINDOWED;
	Window Window;
	Window.OpenWindow(WndClass, "Gamma", 1920, 1080);
	Renderer Renderer;
	Renderer.CreateContext(&Window);
	while (!Window.ShouldClose()) {
		Window.PollEvents();
	}
	Window.CloseWindow();
	GammaQuit();
	return 0;
}