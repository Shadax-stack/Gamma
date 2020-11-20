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

	GAMMA_API void GammaInit(void);
	GAMMA_API void GammaQuit(void);

	//So the IDE can help us better
	namespace Graphics = OpenGL;

}

//A preprocesser define is used to avoid weird errors with namespaces and friend classes
#define Graphics OpenGL

#endif
