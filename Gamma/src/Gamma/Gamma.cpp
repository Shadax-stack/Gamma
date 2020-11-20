// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Gamma.h"
#include <SDL.h>
#include <GL/glew.h>
#include <time.h>

namespace Gamma {

#ifdef GAMMA_DEBUG
	extern uint64_t TimeOffset;
#endif

#ifdef GAMMA_BUILD_D3D9
	namespace Direct3D9 {
		extern void Gamma_Direct3D9_Init(void);
		extern void Gamma_Direct3D9_Quit(void);
	}
#endif

#ifdef GAMMA_BUILD_D3D11
	namespace Direct3D11 {
		extern void Gamma_Direct3D11_Init(void);
	}
#endif

#ifdef GAMMA_BUILD_OPENGL
	namespace OpenGL {
		extern void Gamma_OpenGL_Init(void);
	}
#endif

	void GammaInit(void) {
#ifdef GAMMA_BUILD_OPENGL
		OpenGL::Gamma_OpenGL_Init();
#endif
#ifdef GAMMA_BUILD_D3D9
		Direct3D9::Gamma_Direct3D9_Init();
#endif
		SDL_Init(SDL_INIT_VIDEO);
#ifdef GAMMA_DEBUG
		time_t local, gmt, now = time(nullptr);
		tm TimeInfo;
		localtime_s(&TimeInfo, &now);
		local = mktime(&TimeInfo);
		gmtime_s(&TimeInfo, &now);
		gmt = mktime(&TimeInfo);
		TimeOffset = local - gmt;
#endif
	}

	void GammaQuit(void) {
#ifdef GAMMA_BUILD_D3D9
		Direct3D9::Gamma_Direct3D9_Quit();
#endif
		SDL_Quit();
	}

}