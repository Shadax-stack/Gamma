#include <Gamma/Gamma.h>
#include <Gamma/Window/Window.h>
#include <Gamma/Renderer/OpenGL/Renderer.h>
#include <iostream>

using namespace Gamma;
using namespace Gamma::Graphics;

int main() {
	GammaInit();
	WindowClass WndClass;
	WndClass.FullscreenState = FullscreenState::WINDOWED;
	Window Window;
	Window.OpenWindow(WndClass, "Gamma", 640, 480);
	Renderer Renderer;
	Renderer.CreateContext(&Window);
	while (!Window.ShouldClose()) {
		Renderer.NewFrame();
		Window.PollEvents();
		Renderer.EndFrame();
	}
	Renderer.FreeContext();
	Window.CloseWindow();
	GammaQuit();
	return 0;
}