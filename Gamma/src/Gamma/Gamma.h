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

	GAMMA_API void GammaInit(void);
	GAMMA_API void GammaQuit(void);

}

#endif
