// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Gamma.h"
#include <SDL.h>
#include <time.h>

namespace Gamma {

#ifdef GAMMA_DEBUG
	extern uint64_t TimeOffset;
#endif

	namespace Graphics {
		extern void Gamma_Graphics_API_Init(void);
		extern void Gamma_Graphics_API_Quit(void);
	}

	void GammaInit(void) {
#ifdef GAMMA_DEBUG
		time_t local, gmt, now = time(nullptr);
		tm TimeInfo;
		localtime_s(&TimeInfo, &now);
		local = mktime(&TimeInfo);
		gmtime_s(&TimeInfo, &now);
		gmt = mktime(&TimeInfo);
		TimeOffset = local - gmt;
#endif
		Graphics::Gamma_Graphics_API_Init();
		SDL_Init(SDL_INIT_VIDEO);
	}

	void GammaQuit(void) {
		Graphics::Gamma_Graphics_API_Quit();
		SDL_Quit();
	}

}