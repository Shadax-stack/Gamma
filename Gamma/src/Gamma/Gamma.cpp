// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Gamma.h"
#include <SDL.h>
#include <GL/glew.h>
#include <d3d9.h>

namespace Gamma {

#ifdef GAMMA_BUILD_D3D9
	namespace Direct3D9 {
		extern void Gamma_Direct3D9_Init(void);
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
	}

	void GammaQuit(void) {
		SDL_Quit();
	}

}