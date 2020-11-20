#include "Renderer.h"
#include <SDL_syswm.h>
#include "../../Core/Log.h"
#ifdef GAMMA_PLATFORM_WINDOWS
#include <Windows.h>
#define THREAD_SLEEP(milliseconds) Sleep(milliseconds)
#else
#include <time.h>
// usleep has been deprecated, so nanosleep is used instead
#define THREAD_SLEEP(milliseconds) nanosleep(milliseconds * 1000000)
#endif


namespace Gamma {

	namespace Direct3D9 {

		LPDIRECT3D9 Direct3D9Interface;

		void Gamma_Direct3D9_Init(void) {
			Direct3D9Interface = Direct3DCreate9(D3D_SDK_VERSION);
		}

		void Gamma_Direct3D9_Quit(void) {
			Direct3D9Interface->Release();
		}

		HRESULT CheckFormat(D3DFORMAT fmt, bool windowed) {
			return Direct3D9Interface->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, fmt, fmt, windowed);
		}

		D3DFORMAT FindUsableFormat(bool windowed) {
			D3DDISPLAYMODE DisplayMode;
			HRESULT Result;
			Result = Direct3D9Interface->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode);
			GAMMA_ASSERT_CRITICAL(Result == D3D_OK, "Unable to get the D3DDISPLAYMODE: IDirect3D9::GetAdapterDisplayMode returned %i", Result);
			Result = CheckFormat(DisplayMode.Format, windowed);
			GAMMA_ASSERT_CRITICAL(Result == D3D_OK, "Unable to find a usable D3DFMT: IDirect3D9::CheckDeviceType returned %i", Result);
			GAMMA_INFO("Found usable D3DFMT: %i", DisplayMode.Format);
			return DisplayMode.Format;
		}

		HWND GetWindowHandle(SDL_Window* window) {
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(window, &wmInfo);
			return wmInfo.info.win.window;
		}

#define GAMMA_D3D9_DEVICE_MAX_RECOVERY_ATTEMPTS 16

		void Renderer::RecoverDevice(LPDIRECT3DDEVICE9 device) {
			HRESULT Result;
			uint16_t RecoveryAttempts = 0;
		DeviceRecovery:
			Result = device->TestCooperativeLevel();
			if (Result == D3DERR_DEVICELOST) {
				GAMMA_INFO("IDirect3DDevice9 recovery attempt %i", RecoveryAttempts++);
				while (Result == D3DERR_DEVICELOST) {
					THREAD_SLEEP(1);
					Result = device->TestCooperativeLevel();
				}
				Result = device->Reset(&DevicePresentParameters);
				GAMMA_ASSERT_ERROR(SUCCEEDED(Result), "Failed to reset IDirect3DDevice9: IDirect3DDevice9::Reset returned %i", Result);
				if (FAILED(Result)) {
					if (RecoveryAttempts < GAMMA_D3D9_DEVICE_MAX_RECOVERY_ATTEMPTS) {
						GAMMA_INFO("Attempting to rerecover IDirect3DDevice9 with %i tries remaining", GAMMA_D3D9_DEVICE_MAX_RECOVERY_ATTEMPTS - RecoveryAttempts);
						goto DeviceRecovery;
					}
					else {
						GAMMA_CRITICAL("Failed to recover IDirect3DDevice9");
						psnip_trap();
					}
				}
			}
		}

		void Renderer::CreateContext(Gamma::Window* window) {
			// A D3D9 device needs to be initialized using a D3DPRESENT_PARAMETERS structure
			// We fill out the values and then initalize the device with it
			HRESULT Result;
			// Get the WIN32 API window handle from our SDL2 window
			HWND hWindow = GetWindowHandle(window->pWindow);
			// We actually don't need to set the struct to 0 since all the mmebers will be defined
			// However we will still for debug builds as good programming practice
			#ifdef GAMMA_DEBUG
			ZeroMemory(&DevicePresentParameters, sizeof(D3DPRESENT_PARAMETERS));
			#endif
			// The D3D9 window will be the SDL2 window
			DevicePresentParameters.hDeviceWindow = hWindow;
			// Until I can get SDL2 to work fullscreen our renderer will assum that the window is windowed
			DevicePresentParameters.Windowed = TRUE;
			// Set the back buffer dimensions to the window dimensions
			DevicePresentParameters.BackBufferWidth = window->Width;
			DevicePresentParameters.BackBufferHeight = window->Height;
			// We only want a single back buffer
			DevicePresentParameters.BackBufferCount = 0; // 0 is the same as 1 in this case
			// Use D3DSWAPEFFECT_DISCARD for fast buffer swapping
			DevicePresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			// Use VSYNC for presentation
			DevicePresentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			DevicePresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
			// Use a standard back RGB8 buffer format
			DevicePresentParameters.BackBufferFormat = FindUsableFormat(DevicePresentParameters.Windowed);
			// We will use a depth and stencil buffer when our renderer advances to such stage
			DevicePresentParameters.EnableAutoDepthStencil = FALSE;
			DevicePresentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
			// Currently no flags are going to be used
			DevicePresentParameters.Flags = 0;
			// We do not want any for of multisampling currently
			DevicePresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			DevicePresentParameters.MultiSampleQuality = 0;
			// Create the device
			Result = Direct3D9Interface->CreateDevice(
				D3DADAPTER_DEFAULT,                   // Use the default GPU
				D3DDEVTYPE_HAL,                       // Use HAL instead of software processing 
				hWindow,                              // Create a D3D9 context for the SDL2 window
				D3DCREATE_HARDWARE_VERTEXPROCESSING,  // Use hardware vertex processing for better performance
				&DevicePresentParameters,             // Use the present parameters
				&Device                               // Create the device into the device class member
			);
			// Make sure our device is valid on debug builds
			GAMMA_ASSERT_CRITICAL(Result == D3D_OK, "Unable to create a D3D9 device: IDirect3D9::CreateDevice returned %i", Result);
			GAMMA_INFO("Sucessfuly created a IDirect3DDevice9");
		}

		void Renderer::FreeContext(void) {
			Device->Release();
			GAMMA_INFO("Released a IDirect3DDevice9");
		}

		void Renderer::SwapBuffers(void) {
			RecoverDevice(Device);
			HRESULT Result;
			Result = Device->Present(NULL, NULL, NULL, NULL);
			GAMMA_ASSERT_CRITICAL(Result == D3D_OK, "Unable to present the backbuffer, IDirect3DDevice9::Present returned %i", Result);
		}

		void Renderer::Clear(const Krypton::Vector4f color, float depth, uint8_t stencil) {
			RecoverDevice(Device);
			HRESULT Result;
			D3DCOLOR ClearColor = D3DCOLOR_RGBA((uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255), (uint8_t)(color.a * 255));
			Result = Device->Clear(0, NULL, D3DCLEAR_TARGET, ClearColor, 1.0f, 255);
			GAMMA_ASSERT_CRITICAL(Result == D3D_OK, "Unable to clear the backbuffer: IDirect3DDevice9::Clear returned %i", Result);
		}

		void Renderer::NewFrame(void) {
			RecoverDevice(Device);
			HRESULT Result;
			Krypton::Vector4f color;
			color.r = 0.0f;
			color.g = 0.0f;
			color.b = 0.0f;
			color.a = 0.0f;
			Clear(color);
			Result = Device->BeginScene();
			GAMMA_ASSERT_CRITICAL(Result == D3D_OK, "Unable to begin the scene: IDirect3DDevice9::BeginScene returned %i", Result);
		}

		void Renderer::EndFrame(void) {
			RecoverDevice(Device);
			HRESULT Result;
			Result = Device->EndScene();
			GAMMA_ASSERT_CRITICAL(Result == D3D_OK, "Unable to end the scene: IDirect3DDevice9::EndScene returned %i", Result);
			SwapBuffers();
		}

	}

}