#ifndef GAMMA_OPENGL_RENDERER_H
#define GAMMA_OPENGL_RENDERER_H
#include "../../Gamma.h"
#include "../../Window/Window.h"
#include <SDL.h>
#include <Krypton.h>

namespace Gamma {

	namespace OpenGL {

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

}

#endif