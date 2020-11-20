// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

namespace Gamma {

	void Logger::Info(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		// Why 2 calls to printf? because this is a temporary solution and I will create my own logging library for this
		printf("[GAMMA] Info: ");
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	void Logger::Warn(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		printf("[GAMMA] Warn: ");
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	void Logger::Error(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		printf("[GAMMA] Error: ");
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	void Logger::Critical(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		printf("[GAMMA] Critical: ");
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	Logger GlobalLogger;

}