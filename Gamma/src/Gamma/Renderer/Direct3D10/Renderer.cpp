#include "Renderer.h"
#include "../../Core/Log.h"
#include "../../Win32/Direct3DUtils.h"
#include <Windows.h>
#include <d3d10.h>
#include <dxgi.h>

namespace Gamma {

	namespace Direct3D10 {

		void Gamma_Graphics_API_Init(void) {
			// It appears that D3D10 requires no initailization
			GAMMA_INFO("Initialized D3D10");
		}

		void Gamma_Graphics_API_Quit(void) {
			// We do not need to clean anything up (see the Direct3D10::Gamma_Graphics_API_Init function)
			GAMMA_INFO("Freed D3D10");
		}

		void Renderer::CreateContext(Gamma::Window* window) {
			HRESULT Result;
			#ifdef GAMMA_DEBUG 
			ZeroMemory(&ContextSwapChain.Description, sizeof(DXGI_SWAP_CHAIN_DESC));
			ZeroMemory(&ContextSwapChain.DepthStencilBuffer.Description, sizeof(D3D10_TEXTURE2D_DESC));
			ZeroMemory(&ContextSwapChain.Viewport, sizeof(D3D10_VIEWPORT));
			#endif // GAMMA_DEBUG 
			ContextSwapChain.Dimensions.Width = window->GetDimensions().Width;
			ContextSwapChain.Dimensions.Height = window->GetDimensions().Height;
			// Output to the SDL2 window
			ContextSwapChain.Description.OutputWindow = Win32::GetWindowHandle(window);
			// Find out whether we are creating a fullscreen swapchain
			ContextSwapChain.Description.Windowed = !window->GetFullscreenState();
			// The size of the swap chain is the size of the window
			ContextSwapChain.Description.BufferDesc.Width = ContextSwapChain.Dimensions.Width;
			ContextSwapChain.Description.BufferDesc.Height = ContextSwapChain.Dimensions.Height;
			// TODO: Find a usable format finstead of hard coding it 
			ContextSwapChain.Description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			// For now our application will assume that the monitor is a 60 Hz monitor
			ContextSwapChain.Description.BufferDesc.RefreshRate.Numerator = 60;
			ContextSwapChain.Description.BufferDesc.RefreshRate.Denominator = 1;
			// Unspecified rasterizing
			ContextSwapChain.Description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			// For now it will be centered unless we want to use streched for something like 4:3 on to 16:9, which would be useful in shooter games
			ContextSwapChain.Description.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
			// Disable multisampling
			ContextSwapChain.Description.SampleDesc.Count = 1;
			ContextSwapChain.Description.SampleDesc.Quality = 0;
			// Render to the swapchain with a single back buffer
			ContextSwapChain.Description.BufferCount = 1;
			ContextSwapChain.Description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			// Discard for fast swapping
			ContextSwapChain.Description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			// Use the optimal format in fullscreen
			ContextSwapChain.Description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			Result = D3D10CreateDeviceAndSwapChain(
				NULL,                           // Use the default adapter
				D3D10_DRIVER_TYPE_HARDWARE,     // Use the hardware driver
				NULL,                           // Hardware driver is being used
				0,                              // No flags set
				D3D10_SDK_VERSION,              // Use the SDK version
				&ContextSwapChain.Description,  // Swap chain description info
				&ContextSwapChain.SwapChain,    // Create the swap chain
				&Device                         // Create the device
			);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a device and swap chain, D3D10CreateDeviceAndSwapChain returned %i", Result);
			GAMMA_INFO("Created D3D10 device and DXGI swap chain with size of %i by %i", ContextSwapChain.Description.BufferDesc.Width, ContextSwapChain.Description.BufferDesc.Height);
			// Set up the backbuffer
			Result = ContextSwapChain.SwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&ContextSwapChain.BackBuffer.Texture2D);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to get back buffer 0, IDXGISwapChain::GetBuffer returned %i", Result);
			GAMMA_INFO("Created back buffer");
			// Create the render target view
			Result = Device->CreateRenderTargetView(ContextSwapChain.BackBuffer.Texture2D, NULL, &ContextSwapChain.BackBuffer.RenderTargetView);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a render target view for back buffer 0, ID3D10Device::CreateRenderTargetView returned %i", Result);
			GAMMA_INFO("Created back buffer render target resource view");
			// Set up the depth-stencil buffer
			// Use the dimensions of the swap chain
			ContextSwapChain.DepthStencilBuffer.Description.Width = ContextSwapChain.Dimensions.Width;
			ContextSwapChain.DepthStencilBuffer.Description.Height = ContextSwapChain.Dimensions.Height;
			// A single mip level is required for a depth-stencil buffer
			ContextSwapChain.DepthStencilBuffer.Description.MipLevels = 1;
			// A depth stencil buffer is not an array texture
			ContextSwapChain.DepthStencilBuffer.Description.ArraySize = 1;
			// Use a standard D24_S8 format
			ContextSwapChain.DepthStencilBuffer.Description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			// Match sampling with swap chain sampling
			ContextSwapChain.DepthStencilBuffer.Description.SampleDesc = ContextSwapChain.Description.SampleDesc;
			// Allow default access to the texture
			ContextSwapChain.DepthStencilBuffer.Description.Usage = D3D10_USAGE_DEFAULT;
			// Specify a render target depth-stencil binding flag
			ContextSwapChain.DepthStencilBuffer.Description.BindFlags = D3D10_BIND_DEPTH_STENCIL;
			// CPU read-write access is not required
			ContextSwapChain.DepthStencilBuffer.Description.CPUAccessFlags = 0;
			// No misc flags needed
			ContextSwapChain.DepthStencilBuffer.Description.MiscFlags = 0;
			// Create the depth-stencil texture
			Result = Device->CreateTexture2D(&ContextSwapChain.DepthStencilBuffer.Description, NULL, &ContextSwapChain.DepthStencilBuffer.Texture2D);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a depth-stencil buffer, ID3D10Device::CreateTexture2D returned %i", Result);
			GAMMA_INFO("Created depth-stencil buffer");
			// Create the depth-stencil resource view
			Result = Device->CreateDepthStencilView(ContextSwapChain.DepthStencilBuffer.Texture2D, NULL, &ContextSwapChain.DepthStencilBuffer.DepthStencilView);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a depth-stencil resource view, ID3D10Device::CreateDepthStencilView returned %i", Result);
			GAMMA_INFO("Created depth-stencil buffer render target resource view");
			// Now set the back buffer and depth stencil buffer as the render target and depth stencil resource views
			Device->OMSetRenderTargets(1, &ContextSwapChain.BackBuffer.RenderTargetView, ContextSwapChain.DepthStencilBuffer.DepthStencilView);
			// The vewport does not need to be offsetted from (0, 0)
			ContextSwapChain.Viewport.TopLeftX = 0;
			ContextSwapChain.Viewport.TopLeftY = 0;
			// Use the width and height of window for swap chain
			ContextSwapChain.Viewport.Width = ContextSwapChain.Dimensions.Width;
			ContextSwapChain.Viewport.Height = ContextSwapChain.Dimensions.Height;
			// Default depth ranges
			ContextSwapChain.Viewport.MinDepth = 0.0f;
			ContextSwapChain.Viewport.MaxDepth = 1.0f;
			// Use the viewport
			Device->RSSetViewports(1, &ContextSwapChain.Viewport);
			GAMMA_INFO("Set viewport size to %i by %i", ContextSwapChain.Viewport.Width, ContextSwapChain.Viewport.Height);
		}

		void Renderer::FreeContext(void) {
			ContextSwapChain.BackBuffer.RenderTargetView->Release();
			ContextSwapChain.BackBuffer.Texture2D->Release();
			ContextSwapChain.SwapChain->Release();
			GAMMA_INFO("Freed swap chain");
			Device->Release();
			GAMMA_INFO("Freed D3D10 device");
		}

		void Renderer::SwapBuffers(void) {
			ContextSwapChain.SwapChain->Present(1, 0);
		}

		void Renderer::Clear(const Krypton::Vector4f color, float depth, uint8_t stencil) { 
			Device->ClearRenderTargetView(ContextSwapChain.BackBuffer.RenderTargetView, (const float*)&color);
			Device->ClearDepthStencilView(ContextSwapChain.DepthStencilBuffer.DepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, depth, stencil);
		}

		void Renderer::NewFrame(void) {
			Clear(Krypton::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 255);
		}

		void Renderer::EndFrame(void) {
			SwapBuffers();
		}

	}

}