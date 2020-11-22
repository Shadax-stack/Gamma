// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_H
#define GAMMA_H

#ifdef GAMMA_SHARED_LIBRARY
#ifdef GAMMA_MSVC
#define GAMMA_EXPORT _declspec(dllexport)
#define GAMMA_IMPORT _declspec(dllimport)
#else
#define GAMMA_EXPORT __attribute__((visibility("default")))
#define GAMMA_IMPORT
#endif
#else
#define GAMMA_EXPORT 
#define GAMMA_IMPORT
#endif

#ifdef GAMMA_BUILD
#define GAMMA_API GAMMA_EXPORT
#else
#define GAMMA_API GAMMA_IMPORT
#endif

#define SDL_main_h_

namespace Gamma {

	namespace OpenGL {

		enum class Version {
			OGL_1_1,
			OGL_1_2,
			OGL_1_3,
			OGL_1_4,
			OGL_1_5,
			OGL_2_0,
			OGL_2_1,
			OGL_3_0,
			OGL_3_1,
			OGL_3_2,
			OGL_3_3,
			OGL_4_0,
		};

	}

	namespace Direct3D9 {

		enum class Version {
			DX9,
			DX9a,
			DX9b,
			DX9c,
		};

	}

	namespace Direct3D10 {

		enum class Version {
			//TODO: Add versions for this API
		};

	}

	namespace Direct3D11 {

		enum class Version {
			//TODO: Add versions for this API
		};

	}

	GAMMA_API void GammaInit(void);
	GAMMA_API void GammaQuit(void);

#if defined(GAMMA_BUILD_OPENGL)
	namespace Graphics = OpenGL;
	#define Graphics OpenGL
#elif defined(GAMMA_BUILD_VULKAN)
	namespace Graphics = Vulkan;
	#define Graphics Vulkan
#elif defined(GAMMA_BUILD_D3D9)
	namespace Graphics = Direct3D9;
	#define Graphics Direct3D9
#elif defined(GAMMA_BUILD_D3D10)
	namespace Graphics = Direct3D10;
	#define Graphics Direct3D10
#elif defined(GAMMA_BUILD_D3D11)
	namespace Graphics = Direct3D11;
	#define Graphics Direct3D11
#elif defined(GAMMA_BUILD_D3D12)
	namespace Graphics = Direct3D11;
	#define Graphics Direct3D11
#else
	#error Please select a graphics API in CMake
#endif

}

/*
Include template:

#if defined(GAMMA_BUILD_OPENGL)

#elif defined(GAMMA_BUILD_VULKAN)

#elif defined(GAMMA_BUILD_D3D9)

#elif defined(GAMMA_BUILD_D3D10)

#elif defined(GAMMA_BUILD_D3D11)

#elif defined(GAMMA_BUILD_D3D12)

#else
	#error Please select a graphics API in CMake
#endif

*/

#endif
