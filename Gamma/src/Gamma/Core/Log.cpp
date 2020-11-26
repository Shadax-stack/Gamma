// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

namespace Gamma {


	uint64_t TimeOffset;
	char CurrentTime[16]{ '\0' };

	char* GetCurrentTime(void) { 
		//We use nanos even though we use seconds for when we go for a more "complex" way of showing time
		_timespec64 time;
		int time_get = _timespec64_get(&time, TIME_UTC); 
		uint64_t seconds = time.tv_sec + TimeOffset;
		CurrentTime[0] = '\0'; // Overwrite the previous time
		sprintf_s(CurrentTime, sizeof(CurrentTime), "%lli:%lli:%lli", seconds / 3600 % 24, seconds / 60 % 60, seconds % 60);
		return CurrentTime;
	}

	void Logger::Info(const char* source, const char* fmt, ...) {
		const char* UnknownSource = "Unknown";
		if (!source) {
			source = UnknownSource;
		}
		va_list args;
		va_start(args, fmt);
		// Why 2 calls to printf? because this is a temporary solution and I will create my own logging library for this
		printf("[%s] [GAMMA] [%s] Info: ", GetCurrentTime(), source);
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	void Logger::Warn(const char* source, const char* fmt, ...) {
		const char* UnknownSource = "Unknown";
		if (!source) {
			source = UnknownSource;
		}
		va_list args;
		va_start(args, fmt);
		printf("[%s] [GAMMA] [%s] Warn: ", GetCurrentTime(), source);
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	void Logger::Error(const char* source, const char* fmt, ...) {
		const char* UnknownSource = "Unknown";
		if (!source) {
			source = UnknownSource;
		}
		va_list args;
		va_start(args, fmt);
		printf("[%s] [GAMMA] [%s] Error: ", GetCurrentTime(), source);
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	void Logger::Critical(const char* source, const char* fmt, ...) {
		const char* UnknownSource = "Unknown";
		if (!source) {
			source = UnknownSource;
		}
		va_list args;
		va_start(args, fmt);
		printf("[%s] [GAMMA] [%s] Critical: ", GetCurrentTime(), source);
		vprintf(fmt, args);
		putchar('\n');
		va_end(args);
	}

	Logger GlobalLogger;

}