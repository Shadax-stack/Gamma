#include "Renderer.h"
#include "../../Win32/Direct3DUtils.h"
#include "../../Core/Log.h"

namespace Gamma {

	namespace Direct3D11 {

		void Renderer::CreateContext(Gamma::Window* window) {
			HRESULT Result;
			#ifdef GAMMA_DEBUG
			ZeroMemory(&SwapChain.Description, sizeof(DXGI_SWAP_CHAIN_DESC));
			#endif
			// Support D3D11.0 only currently
			D3D_FEATURE_LEVEL D3D11FeatureLevels[1]{ D3D_FEATURE_LEVEL_11_0 };
			D3D_FEATURE_LEVEL D3D11FeatureLevelSupport;
			// Create the device and device context
			Result = D3D11CreateDevice(
				NULL,                                                     // Use the default adapter
				D3D_DRIVER_TYPE_HARDWARE,                                 // Use hardware acceleration
				NULL,                                                     // Software mode is not requested
				D3D11_CREATE_DEVICE_SINGLETHREADED,                       // TODO: Add multithreading support and swith to using a deferred context
				D3D11FeatureLevels,                                       // Find a supported feature level
				sizeof(D3D11FeatureLevels) / sizeof(D3D_FEATURE_LEVEL),   // All feature levels in D3D11FeatureLevels will be checked for support
				D3D11_SDK_VERSION,                                        // Pass in the current DirectX SDK version
				&Device,                                                  // Create the device
				&D3D11FeatureLevelSupport,                                // Check whether the supported feature level (if any) is D3D11.X
				&DeviceContext                                            // Create an immediate context
			);
			GAMMA_ASSERT_CRITICAL(D3D11FeatureLevelSupport == D3D_FEATURE_LEVEL_11_0, "Unable to create a D3D11 device and device context, D3D_FEATURE_LEVEL_11_0 is not supported");
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a D3D11 device and device context, D3D11CreateDevice returned %#010x", Result);
			GAMMA_INFO("Initialized a D3D11 device and device context");
			// Create the DXGI interfaces and factory
			Result = Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGI.Device);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a DXGI device, ID3D11Device::QueryInterface returned %#010x", Result);
			Result = DXGI.Device->GetParent(__uuidof(IDXGIAdapter), (void**)&DXGI.Adapter);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a DXGI adapter, IDXGIDevice::GetParent returned %#010x", Result);
			Result = DXGI.Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&DXGI.Factory);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a DXGI factory, IDXGIAdapter::GetParent returned %#010x", Result);
			SwapChain.Dimensions.Width = window->GetDimensions().Width;
			SwapChain.Dimensions.Height = window->GetDimensions().Height;
			// Output to the SDL2 window
			SwapChain.Description.OutputWindow = Win32::GetWindowHandle(window);
			// Find out whether we are creating a fullscreen swapchain
			SwapChain.Description.Windowed = !window->GetFullscreenState();
			// The size of the swap chain is the size of the window
			SwapChain.Description.BufferDesc.Width = SwapChain.Dimensions.Width;
			SwapChain.Description.BufferDesc.Height = SwapChain.Dimensions.Height;
			// TODO: Find a usable format finstead of hard coding it 
			SwapChain.Description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			// For now our application will assume that the monitor is a 60 Hz monitor
			SwapChain.Description.BufferDesc.RefreshRate.Numerator = 60;
			SwapChain.Description.BufferDesc.RefreshRate.Denominator = 1;
			// Unspecified rasterizing
			SwapChain.Description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			// For now it will be centered unless we want to use streched for something like 4:3 on to 16:9, which would be useful in shooter games
			SwapChain.Description.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
			// Disable multisampling
			SwapChain.Description.SampleDesc.Count = 1;
			SwapChain.Description.SampleDesc.Quality = 0;
			// Render to the swapchain with a single back buffer
			SwapChain.Description.BufferCount = 1;
			SwapChain.Description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			// Discard for fast swapping
			SwapChain.Description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			// Use the optimal format in fullscreen
			SwapChain.Description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			// Create the swapchain
			DXGI.Factory->CreateSwapChain(Device, &SwapChain.Description, &SwapChain.SwapChain);
			SwapChain.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&SwapChain.BackBuffer.Texture2D);
			GAMMA_ASSERT_CRITICAL(Result == S_OK && SwapChain.BackBuffer.Texture2D != NULL, "Unable to get back buffer 0, IDXGISwapChain::GetBuffer returned %#010x", Result);
			GAMMA_INFO("Created back buffer");
			Device->CreateRenderTargetView(SwapChain.BackBuffer.Texture2D, NULL, &SwapChain.BackBuffer.RenderTargetView);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a render target view for back buffer 0, ID3D10Device::CreateRenderTargetView returned %#010x", Result);
			GAMMA_INFO("Created back buffer render target resource view");
			// Set up the depth-stencil buffer
			// Use the dimensions of the swap chain
			SwapChain.DepthStencilBuffer.Description.Width = SwapChain.Dimensions.Width;
			SwapChain.DepthStencilBuffer.Description.Height = SwapChain.Dimensions.Height;
			// A single mip level is required for a depth-stencil buffer
			SwapChain.DepthStencilBuffer.Description.MipLevels = 1;
			// A depth stencil buffer is not an array texture
			SwapChain.DepthStencilBuffer.Description.ArraySize = 1;
			// Use a standard D24_S8 format
			SwapChain.DepthStencilBuffer.Description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			// Match sampling with swap chain sampling
			SwapChain.DepthStencilBuffer.Description.SampleDesc = SwapChain.Description.SampleDesc;
			// Allow default access to the texture
			SwapChain.DepthStencilBuffer.Description.Usage = D3D11_USAGE_DEFAULT;
			// Specify a render target depth-stencil binding flag
			SwapChain.DepthStencilBuffer.Description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			// CPU read-write access is not required
			SwapChain.DepthStencilBuffer.Description.CPUAccessFlags = 0;
			// No misc flags needed
			SwapChain.DepthStencilBuffer.Description.MiscFlags = 0;
			// Create the depth-stencil texture
			Result = Device->CreateTexture2D(&SwapChain.DepthStencilBuffer.Description, NULL, &SwapChain.DepthStencilBuffer.Texture2D);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a depth-stencil buffer, ID3D11Device::CreateTexture2D returned %#010x", Result);
			GAMMA_INFO("Created depth-stencil buffer");
			// Create the depth-stencil resource view
			Result = Device->CreateDepthStencilView(SwapChain.DepthStencilBuffer.Texture2D, NULL, &SwapChain.DepthStencilBuffer.DepthStencilView);
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "Unable to create a depth-stencil resource view, ID3D11Device::CreateDepthStencilView returned %#010x", Result);
			GAMMA_INFO("Created depth-stencil buffer render target resource view");
			DeviceContext->OMSetRenderTargets(1, &SwapChain.BackBuffer.RenderTargetView, SwapChain.DepthStencilBuffer.DepthStencilView);
			// The vewport does not need to be offsetted from (0, 0)
			SwapChain.Viewport.TopLeftX = 0;
			SwapChain.Viewport.TopLeftY = 0;
			// Use the width and height of window for swap chain
			SwapChain.Viewport.Width = (float)SwapChain.Dimensions.Width;
			SwapChain.Viewport.Height = (float)SwapChain.Dimensions.Height;
			// Default depth ranges
			SwapChain.Viewport.MinDepth = 0.0f;
			SwapChain.Viewport.MaxDepth = 1.0f;
			// Use the viewport
			DeviceContext->RSSetViewports(1, &SwapChain.Viewport);
			GAMMA_INFO("Set viewport size to %i by %i", SwapChain.Viewport.Width, SwapChain.Viewport.Height);
		}

		void Renderer::FreeContext(void) {
			SwapChain.BackBuffer.RenderTargetView->Release();
			SwapChain.BackBuffer.Texture2D->Release();
			SwapChain.DepthStencilBuffer.DepthStencilView->Release();
			SwapChain.DepthStencilBuffer.Texture2D->Release();
			SwapChain.SwapChain->Release();
			GAMMA_INFO("Freed swap chain");
			DXGI.Factory->Release();
			DXGI.Adapter->Release();
			DXGI.Device->Release();
			GAMMA_INFO("Freed DXGI interfaces");
			DeviceContext->Release();
			Device->Release();
			GAMMA_INFO("Freed D3D11 device and device context");
		}

		void Renderer::SwapBuffers(void) {
			SwapChain.SwapChain->Present(1, 0);
		}

		void Renderer::Clear(const Krypton::Vector4f color, float depth, uint8_t stencil) {
			DeviceContext->ClearRenderTargetView(SwapChain.BackBuffer.RenderTargetView, (const float*)&color);
			DeviceContext->ClearDepthStencilView(SwapChain.DepthStencilBuffer.DepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, depth, stencil);
		}

		void Renderer::NewFrame(void) {
			Clear(Krypton::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 255);
		}

		void Renderer::EndFrame(void) {
			SwapBuffers();

		}

		void Gamma_Graphics_API_Init(void) {
			// It appears that D3D10 requires no initailization
			GAMMA_INFO("Initialized D3D11");
		}

		void Gamma_Graphics_API_Quit(void) {
			// We do not need to clean anything up (see the Direct3D10::Gamma_Graphics_API_Init function)
			GAMMA_INFO("Freed D3D11");
		}

	}

}