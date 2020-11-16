#include <Gamma/Gamma.h>
#include <Gamma/Window/Window.h>
#include <iostream>

using namespace Gamma;

int main() {
	GammaInit();
	WindowClass WndClass;
	Window Window;
	Window.OpenWindow(WndClass, "Gamma", 1280, 720);
	GammaQuit();
	return 0;
}