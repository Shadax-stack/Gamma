// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_RENDERER_H
#define GAMMA_RENDERER_H

#include "../Gamma.h"
#include "../Window/Window.h"

namespace Gamma {

	class GAMMA_API Renderer {
	public:
		void CreateContext(Gamma::Window* window);
		void FreeContext(void);
		void SwapBuffers(void);
	private:
		Window* Window;
		SDL_GLContext Context;
	};

}

#endif
