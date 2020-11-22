#ifndef GAMMA_D3D10_RENDERER_H
#define GAMMA_D3D10_RENDERER_H

#include "../../Gamma.h"
#include "../../Window/Window.h"
#include <d3d10.h>
#include <Krypton.h>
#include <stdint.h>

namespace Gamma {

	namespace Direct3D10 {

		class GAMMA_API Renderer {
		public:
			void CreateContext(Gamma::Window* window);
			void FreeContext(void);
			void SwapBuffers(void);
			void Clear(const Krypton::Vector4f color, float depth = 1.0f, uint8_t stencil = 255);
			void NewFrame(void);
			void EndFrame(void);
		private:
			Window* Window;
			ID3D10Device* Device;
			struct {
				DXGI_SWAP_CHAIN_DESC Description;
				IDXGISwapChain* SwapChain;
				struct {
					ID3D10Texture2D* Texture2D;
					ID3D10RenderTargetView* RenderTargetView;
				} BackBuffer;
				struct {
					D3D10_TEXTURE2D_DESC Description;
					ID3D10Texture2D* Texture2D;
					ID3D10DepthStencilView* DepthStencilView;
				} DepthStencilBuffer;
				D3D10_VIEWPORT Viewport;
				Rectangle Dimensions;
			} ContextSwapChain;
		};

		

	}

}

#endif // !GAMMA_D3D10_RENDERER_H

