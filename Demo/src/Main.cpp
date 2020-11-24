#include <Gamma/Gamma.h>
#include <Gamma/Window/Window.h>
#include <Gamma/Renderer/Renderer.h>
#include <Gamma/Renderer/Factory.h>
#include <Gamma/Renderer/Mesh.h>
#include <iostream>

using namespace Gamma;
using namespace Gamma::Graphics;

ScalarVertex Triangle[3] = {
	{ScalarVec3f(1.0f, 1.0f, 0.0f)},
	{ScalarVec3f(1.0f, 0.0f, 0.0f)},
	{ScalarVec3f(0.0f, 1.0f, 0.0f)},
};

// An application class is not used to allow the user to do whatever they want for their entry point
// In a real game engine a user should not have to worry about the entry point, any programming done would be scripting (or other things like shader programming but that is not a current goal for this engine)
// So this could be seen as a temporary solution
int main() {
	GammaInit();
	WindowClass WndClass;
	WndClass.FullscreenState = FullscreenState::WINDOWED;
	Window Window;
	Window.OpenWindow(WndClass, "Gamma", 640, 480);
	Renderer Renderer;
	Renderer.CreateContext(&Window);
	Factory* Factory = Renderer.GetFactory();
	Mesh* Mesh = Factory->CreateMesh(sizeof(Triangle) / sizeof(ScalarVertex), Triangle);
	while (!Window.ShouldClose()) {
		Renderer.NewFrame();
		Renderer.Draw(Mesh);
		Window.PollEvents();
		Renderer.EndFrame();
	}
	Mesh->Release();
	Renderer.FreeContext();
	Window.CloseWindow();
	GammaQuit();
	return 0;
}