#ifndef GAMMA_RENDERER_H
#define GAMMA_RENDERER_H

#include "../Gamma.h"
#include "../Window/Window.h"

namespace Gamma {

	class GAMMA_API Renderer {
	public:
		void CreateContxt(Window* window);
		void FreeContext(void);
	private:
		SDL_GLContext Context;
	};

}

#endif
