#ifndef GAMMA_DIRECT3D9_RENDERER_H
#define GAMMA_DIRECT3D9_RENDERER_H

#include "../../Gamma.h"
#include "../../Window/Window.h"
#include <d3d9.h>
#include <Krypton.h>

namespace Gamma {

	namespace Direct3D9 {

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
			LPDIRECT3DDEVICE9 Device;
		};

	}

}

#endif // !GAMMA_DIRECT3D9_RENDERER_H

