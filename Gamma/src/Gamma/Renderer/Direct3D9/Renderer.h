#ifndef GAMMA_DIRECT3D9_RENDERER_H
#define GAMMA_DIRECT3D9_RENDERER_H

#include "../../Gamma.h"
#include "../../Window/Window.h"
#include <d3d9.h>
#include <Krypton.h>
#include <stdint.h>

namespace Gamma {

	namespace Direct3D9 {

		class GAMMA_API Renderer {
		public:
			void CreateContext(Gamma::Window* window);
			void FreeContext(void);
			void SwapBuffers(void);
			void Clear(const Krypton::Vector4f color, float depth = 1.0f, uint8_t stencil = 255);
			void NewFrame(void);
			void EndFrame(void);
		private:
			void RecoverDevice(LPDIRECT3DDEVICE9 device);
			Window* Window;
			LPDIRECT3DDEVICE9 Device;
			D3DPRESENT_PARAMETERS DevicePresentParameters;
		};

	}

}

#endif // !GAMMA_DIRECT3D9_RENDERER_H

