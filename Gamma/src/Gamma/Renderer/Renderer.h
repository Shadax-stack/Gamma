// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_RENDERER_H
#define GAMMA_RENDERER_H

#include "../Gamma.h"
#include "../Window/Window.h"
#ifdef GAMMA_GRAPHICS_API_DIRECT_X
#include <d3d12.h>
#include <d3d11.h>
#include <d3d9.h>
#endif
#include <Krypton.h>

namespace Gamma {

	class GAMMA_API Renderer {
	public:
		void CreateContext(Gamma::Window* window);
		void FreeContext(void);
		void SwapBuffers(void);
		void ClearColor(const Krypton::Vector4f color);
		void NewFrame(void);
		void EndFrame(void);
	private:
		Window* Window;
		SDL_GLContext Context;
	};

}

#endif
