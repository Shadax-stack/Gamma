#include "Renderer.h"
#include <SDL_syswm.h>
#include "../../Core/Log.h"

namespace Gamma {

	namespace Direct3D9 {

		LPDIRECT3D9 Direct3D9Interface;

		void Gamma_Direct3D9_Init(void) {
			Direct3D9::Direct3D9Interface = Direct3DCreate9(D3D_SDK_VERSION);
		}

		HWND GetWindowHandle(SDL_Window* window) {
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(window, &wmInfo);
			return wmInfo.info.win.window;
		}

		void Renderer::CreateContext(Gamma::Window* window) {
			// Get the WIN32 API window handle from our SDL2 window
			HWND hWindow = GetWindowHandle(window->pWindow);
			// A D3D9 device needs to be initialized using a D3DPRESENT_PARAMETERS structure
			// We fill out the values and then initalize the device with it
			D3DPRESENT_PARAMETERS DevicePresentParameters;
			// We actually don't need to set the struct to 0 since all the mmebers will be defined
			// However we will still for debug builds as good programming practice
			#ifdef GAMMA_DEBUG
			ZeroMemory(&DevicePresentParameters, sizeof(D3DPRESENT_PARAMETERS));
			#endif
			// The D3D9 window will be the SDL2 window
			DevicePresentParameters.hDeviceWindow = hWindow;
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
			DevicePresentParameters.BackBufferFormat = D3DFMT_R8G8B8;
			// We will use a depth and stencil buffer when our renderer advances to such stage
			DevicePresentParameters.EnableAutoDepthStencil = FALSE;
			DevicePresentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
			// Until I can get SDL2 to work fullscreen our renderer will assum that the window is windowed
			DevicePresentParameters.Windowed = TRUE;
			// Currently no flags are going to be used
			DevicePresentParameters.Flags = 0;
			// We do not want any for of multisampling currently
			DevicePresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			DevicePresentParameters.MultiSampleQuality = 0;
			// Create the device
			HRESULT DeviceCreationResult = Direct3D9Interface->CreateDevice(
				D3DADAPTER_DEFAULT,                   // Use the default GPU
				D3DDEVTYPE_HAL,                       // Use HAL instead of software processing 
				hWindow,                              // Create a D3D9 context for the SDL2 window
				D3DCREATE_HARDWARE_VERTEXPROCESSING,  // Use hardware vertex processing for better performance
				&DevicePresentParameters,             // Use the present parameters
				&Device                               // Create the device into the device class member
			);
			// Make sure our device is valid on debug builds
			GAMMA_ASSERT_CRITICAL(DeviceCreationResult == D3D_OK, "Unable to create a D3D9 device: IDirect3D9::CreateDevice returned ", DeviceCreationResult);
		}

		void Renderer::FreeContext(void) {

		}

	}

}