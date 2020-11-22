#ifndef GAMMA_D3D11_RENDERER_H
#define GAMMA_D3D11_RENDERER_H

#include "../../Gamma.h"
#include "../../Window/Window.h"
#include <d3d11.h>
#include <Krypton/Vector.h>

namespace Gamma {

	namespace Direct3D11 {

		class GAMMA_API Renderer {
		public:
			void CreateContext(Gamma::Window* window);
			void FreeContext(void);
			void SwapBuffers(void);
			void Clear(const Krypton::Vector4f color, float depth = 1.0f, uint8_t stencil = 255);
			void NewFrame(void);
			void EndFrame(void);
		private:
			ID3D11Device* Device;
			ID3D11DeviceContext* DeviceContext;
			struct {
				DXGI_SWAP_CHAIN_DESC Description;
				IDXGISwapChain* SwapChain;
				Rectangle Dimensions;
				struct {
					ID3D11Texture2D* Texture2D;
					ID3D11RenderTargetView* RenderTargetView;
				} BackBuffer;
				struct {
					D3D11_TEXTURE2D_DESC Description;
					ID3D11Texture2D* Texture2D;
					ID3D11DepthStencilView* DepthStencilView;
				} DepthStencilBuffer;
				D3D11_VIEWPORT Viewport;
			} SwapChain;
			struct {
				IDXGIDevice* Device;
				IDXGIAdapter* Adapter;
				IDXGIFactory* Factory;
			} DXGI;
		};

	}

}

#endif
