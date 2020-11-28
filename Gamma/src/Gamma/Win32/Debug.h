#ifndef GAMMA_WIN32_DEBUG_H
#define GAMMA_WIN32_DEBUG_H

#include <stdint.h>

namespace Gamma {
	namespace Win32 {

		//Returns true if the user selected the retry option
		bool DebugReport(const char* file, uint32_t line, const char* source, const char* message, ...);

	}
}

#endif // !GAMMA_WIN32_DEBUG_H

