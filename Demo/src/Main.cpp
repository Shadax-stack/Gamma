#include <Gamma/Gamma.h>
#include <Gamma/Window/Window.h>
#include <Gamma/Renderer/Direct3D12/Context.h>
#include <iostream>

using namespace Gamma;
using namespace Gamma::Graphics;


// An application class is not used to allow the user to do whatever they want for their entry point
// In a real game engine a user should not have to worry about the entry point, any programming done would be scripting (or other things like shader programming but that is not a current goal for this engine)
// So this could be seen as a temporary solution
int main() {
	GammaInit();
	WindowClass WndClass;
	WndClass.FullscreenState = FullscreenState::FULLSCREEN;
	Window Window;
	Window.OpenWindow(WndClass, "Gamma");
	Context Context;
	Context.Initialize(&Window);
	while (!Window.ShouldClose()) {
		Context.NewFrame();
		Context.EndFrame();
		Window.PollEvents();
	}
	Window.CloseWindow();
	GammaQuit();
	return 0;
}