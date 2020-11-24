#ifndef GAMMA_OPENGL_RENDERER_H
#define GAMMA_OPENGL_RENDERER_H
#include "../../Gamma.h"
#include "../../Window/Window.h"
#include "Factory.h"
#include <SDL.h>
#include <Krypton.h>

namespace Gamma {

	namespace OpenGL {

		class GAMMA_API Renderer {
		public:
			void CreateContext(Gamma::Window* window);
			void FreeContext(void);
			void SwapBuffers(void);
			void Clear(const Krypton::Vector4f color, float depth = 1.0f, uint8_t stencil = 255);
			void NewFrame(void);
			void EndFrame(void);
			Factory* GetFactory(void);
			void Draw(Mesh* mesh);
		private:
			Window* Window;
			SDL_GLContext Context;
			Factory Factory;
		};

	}

}

#endif