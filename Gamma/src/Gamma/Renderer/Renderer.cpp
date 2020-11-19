// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Renderer.h"
#include <GL/glew.h>

namespace Gamma {

	void Renderer::CreateContext(Gamma::Window* window) {
		Context = SDL_GL_CreateContext(window->pWindow);
		Window = window;
		SDL_GL_SetSwapInterval(1);
		glewInit();
	}

	void Renderer::FreeContext(void) {
		SDL_GL_DeleteContext(Context);
	}

	void Renderer::SwapBuffers(void) {
		SDL_GL_SwapWindow(Window->pWindow);
	}

	void Renderer::ClearColor(const Krypton::Vector4f color) {
		glClearBufferfv(GL_COLOR, 0, (const float*)&color);
	}

	void Renderer::NewFrame(void) {
		Krypton::Vector4f defaultColor;
		defaultColor.r = 1.0f;
		defaultColor.g = 1.0f;
		defaultColor.b = 1.0f;
		defaultColor.a = 1.0f;
		ClearColor(defaultColor);
	}

	void Renderer::EndFrame(void) {
		SwapBuffers();
	}

}