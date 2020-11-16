#include "Renderer.h"

namespace Gamma {

	void Renderer::CreateContxt(Window* window) {
		Context = SDL_GL_CreateContext(window->pWindow);
	}

	void Renderer::FreeContext(void) {
		SDL_GL_DeleteContext(Context);
	}

}