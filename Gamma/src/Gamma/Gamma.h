// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_H
#define GAMMA_H

#ifdef GAMMA_BUILD
#ifdef GAMMA_MSVC
#define GAMMA_API _declspec(dllexport)
#else 
#define GAMMA_API __attribute__((visibility("default")))
#endif
#else
#ifdef GAMMA_MSVC
#define GAMMA_API _declspec(dllimport)
#else
#define GAMMA_API
#endif
#endif

#define SDL_main_h_

namespace Gamma {

	GAMMA_API void GammaInit(void);
	GAMMA_API void GammaQuit(void);

}

#endif
