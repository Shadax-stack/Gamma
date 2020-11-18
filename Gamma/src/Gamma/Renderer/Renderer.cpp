// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Renderer.h"

namespace Gamma {

	void Renderer::CreateContext(Gamma::Window* window) {
		Context = SDL_GL_CreateContext(window->pWindow);
		Window = window;
		SDL_GL_SetSwapInterval(1);
	}

	void Renderer::FreeContext(void) {
		SDL_GL_DeleteContext(Context);
	}

	void Renderer::SwapBuffers(void) {
		SDL_GL_SwapWindow(Window->pWindow);
	}

}