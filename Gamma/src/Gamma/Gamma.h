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
