#include "Debug.h"
#include <debug-trap/debug-trap.h>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#ifdef GAMMA_MSVC
#pragma warning(disable : 4996) // Using the "safe" functions actually cased a stack corruption
#endif // GAMMA_MSVC

namespace Gamma {
	namespace Win32 {

		bool DebugReport(const char* file, uint32_t line, const char* source, const char* message, ...) {
			char ErrorReport[2048];
			ErrorReport[0] = '\0';
			sprintf(ErrorReport, "Gamma Engine Debug Report:\n\nAssertion error at %s at line %u:\n\nMessage: [%s] ", file, line, source);
			va_list list;
			va_start(list, message);
			vsprintf(ErrorReport + strlen(ErrorReport), message, list);
			va_end(list);
			sprintf(ErrorReport + strlen(ErrorReport), "\n\nPress \"Abort\" to abort the application\nPress \"Retry\" to debug the application\nPress \"Ignore\" to ignore the debug report\n");
			switch (MessageBoxA(NULL, ErrorReport, "Gamma Engine Debug Report", MB_ABORTRETRYIGNORE | MB_ICONERROR)) {
			case IDABORT:
				abort();
				return true; // To supress C4715
			case IDIGNORE:
				return false;
			case IDRETRY:
				return true;
			default:
				return true;
			}

		}

	}
}